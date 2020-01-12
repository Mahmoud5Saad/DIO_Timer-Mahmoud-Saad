/******************************************************************************
 *
 * 	Module: 		TMU
 *
 *	File Name: 		TMU.c
 *
 * 	Description: 	source file for Timer management unit
 *
 * 	Authors: 		Mahmoud Saad
 * 	
 *	Created on:		Saturday, 11-Jan-20 3:43:18 PM
 *
 *******************************************************************************/

/*******************************************************************************
 *			INCLUDES                                    
 *******************************************************************************/
#include "TMU.h"
#include "TMU_Cfg.h"
#include "Timer.h" /* TMU uses timer module */
/*******************************************************************************
 *			Preprocessor Macros                                    
 *******************************************************************************/
/*******************************************************************************
 *			Enumerations and structures declarations                                  
 *******************************************************************************/
/* enum to hold module states */
typedef enum TMU_State
{
	TMU_DEINIT, TMU_INIT
}TMU_State;

/* structure to hold entries of each delay service */
typedef struct delayTaskBlock
{
	void (*pCbk_func)(void);
	uint32 delay;
	uint32 period;
}delayTaskBlock;
/*******************************************************************************
 *			Functions Prototypes                                   
 *******************************************************************************/
/*******************************************************************************
 *			Private functions Prototypes                                   
 *******************************************************************************/
/*******************************************************************************
 *			Global variables
 *******************************************************************************/
/* array of delay services details */
delayTaskBlock static ga_delayTasks[MAX_NUMBER_OF_DLAYED_TASKS] = {{NULL,0,0}};
/* global ticks of module */
volatile static uint32 gu32_delayTicks = 0;
/* top of our delay list */
static uint32 gu32_currentFirstEmpty = 0;
/* global enum for module state */
TMU_State static ge_modulestate = TMU_DEINIT;
/*******************************************************************************
 *			Static global variables
 *******************************************************************************/
/*******************************************************************************
 *			Extern Global variables
 *******************************************************************************/
 /*******************************************************************************
 *			Functions Definitions                                  
 *******************************************************************************/

/*******************************************************************************
 * 	Function name:		delayTicks_Increment
 *
 * 	Brief:				increments ticks of module
 *
 * 	Details:			called by timer ISR by hardware to update ticks
 *
 * 	Arguments:
 * 		-				void
 *
 * 	Return:
 * 		-				void
 *******************************************************************************/
void delayTicks_Increment(void)
{
	gu32_delayTicks++;
}

/*******************************************************************************
 * 	Function name: 		TMU_Init
 *
 * 	Brief: 				initialize TMU module
 *
 * 	Details: 			initialize TMU module and
 *
 * 	Arguments:
 * 		-				pointer to config_TMU_s structure which has 2 elements:
 *						-timer_ch timer channel
  *						-resoltuion_ticks resolution of smallest delay
 *
 * 	Return:
 * 		-				ERROR_STATUS to report success or failure
 *******************************************************************************/
ERROR_STATUS TMU_Init(config_TMU_s *cfgPtr)
{
	/* local declarations */
	uint8 timer_counts = 0;
	if ( NULL == cfgPtr )
	{
		return E_NOK;
	}
	else
	{
		/* zero global ticks of module */
		gu32_delayTicks = 0;
		/*
		res is in milliseconds
		prescalar is TIMER_PRESCALER_64
		means one tick is 64 processor tick
		time of 1 tick = prescalar / F_CPU = 64 / 16M = 4 micro seconds
		then divide resolution time on tick time and give the value to the timer to count.
		 */
		timer_counts = (1000*(cfgPtr->resoltuion_ticks))/4 ;
		Timer_cfg_s timer_TMU_init = 
		{
			cfgPtr->timer_ch,
			TIMER_MODE,
			TIMER_INTERRUPT_MODE,
			TIMER_PRESCALER_64,
			&delayTicks_Increment
		};
		/* init timer and give it the value to count */
		Timer_Init(&timer_TMU_init);
		Timer_Start(cfgPtr->timer_ch,timer_counts);
		return E_OK;
	}
}
/*******************************************************************************
 * 	Function name: 		TMU_Dispatcher
 *
 * 	Brief: 				dispatcher TMU module
 *
 * 	Details: 			dispatches delay services that are due in TMU module
 *						and notify the user using the pointers to functions 
 *						stored in its array
 *
 * 	Arguments:
 * 		-				void
 *
 * 	Return:
 * 		-				ERROR_STATUS to report success or failure
 *******************************************************************************/
ERROR_STATUS TMU_Dispatcher()
{
	uint32 i = 0;
	/* if there are unconsumed ticks, consume them */
	if (gu32_delayTicks>0)
	{
		/* atomic process, disable and enable interrupts */
		cli();
		gu32_delayTicks--;
		sei();
		/* loop on filled part of the array of structures */
		for (i=0; i<gu32_currentFirstEmpty; i++)
		{
			/* if pointer to function is not empty */
			if ( 0 != (ga_delayTasks[i].pCbk_func) )
			{
				if (ga_delayTasks[i].delay == 0) /* waiting is over */
				{
					(*(ga_delayTasks[i].pCbk_func))(); /* notify the user */
					(ga_delayTasks[i].delay) = (ga_delayTasks[i].period);
					if ( (ga_delayTasks[i].period) == 0) /* if one time task*/
					{
						/* remove from tasks */
						ga_delayTasks[i].pCbk_func = ga_delayTasks[gu32_currentFirstEmpty-1].pCbk_func;
						ga_delayTasks[i].delay = ga_delayTasks[gu32_currentFirstEmpty-1].delay;
						ga_delayTasks[i].period = ga_delayTasks[gu32_currentFirstEmpty-1].period;
						gu32_currentFirstEmpty--;
					}
				}
				else /* still waiting */
				{
					/* reduce delay period */
					--(ga_delayTasks[i].delay);
				}
			}
		}
	}
	return E_OK;
}
/*******************************************************************************
 * 	Function name: 		TMU_Start_time
 *
 * 	Brief: 				Starts a delay service in TMU
 *
 * 	Details: 			Starts a task to notify the user after a specific 
 *						delay period
 *
 * 	Arguments:
 * 		-				n_Cbk_delayTask pointer to func to notify the user
 * 		-				n_delay the period to notify after in the first time
 * 		-				n_period  the period to notify after in subsequest times
 *							-if 0, it means one time task, not periodic
 *
 * 	Return:
 * 		-				the index of the task created
 *******************************************************************************/
uint32 TMU_Start_time(void (*n_Cbk_delayTask)(void),uint32 n_delay, uint32 n_period)
{
	/* index of delay service */
	uint8 index = 0;
	if ( NULL == n_Cbk_delayTask)
	{
		return E_NOK;
	} 
	else
	{
		/* add delay service */
		ga_delayTasks[gu32_currentFirstEmpty].pCbk_func = n_Cbk_delayTask;
		ga_delayTasks[gu32_currentFirstEmpty].delay = n_delay;
		ga_delayTasks[gu32_currentFirstEmpty].period = n_period;
		index = gu32_currentFirstEmpty;
		gu32_currentFirstEmpty++;
		/* return index to user so that he can use later for reference */
		return index;
	}
}
/*******************************************************************************
 * 	Function name: 		TMU_Stop_Time
 *
 * 	Brief: 				Stops a delay service in TMU
 *
 * 	Details: 			stops a task to notify the user after a specific 
 *						delay period
 *
 * 	Arguments:
 * 		-				deleteByIndex: index of task to be deleted

 *
 * 	Return:
 * 		-				return success or failure
 *******************************************************************************/
ERROR_STATUS TMU_Stop_Time(uint32 deleteByIndex)
{
	if (deleteByIndex>gu32_currentFirstEmpty)
	{
		/* return error if wrong index */
		return E_NOK;
	} 
	else
	{
		/* remove task from array */
		ga_delayTasks[deleteByIndex].pCbk_func = NULL;
		ga_delayTasks[deleteByIndex].period = 0;
		ga_delayTasks[deleteByIndex].delay = 0;
		if (deleteByIndex==(gu32_currentFirstEmpty-1))
		{
			/* if last task was deleted return */
			gu32_currentFirstEmpty--;
			return E_OK;
		} 
		else
		{
			/* if not last task, copy last task in its place */
			ga_delayTasks[deleteByIndex].pCbk_func = ga_delayTasks[gu32_currentFirstEmpty-1].pCbk_func;
			ga_delayTasks[deleteByIndex].delay = ga_delayTasks[gu32_currentFirstEmpty-1].delay;
			ga_delayTasks[deleteByIndex].period = ga_delayTasks[gu32_currentFirstEmpty-1].period;
			/* delete the copied task */
			ga_delayTasks[gu32_currentFirstEmpty-1].pCbk_func = 0;
			ga_delayTasks[gu32_currentFirstEmpty-1].delay = 0;
			ga_delayTasks[gu32_currentFirstEmpty-1].period = 0;
			/* decrement the top of tasks*/
			gu32_currentFirstEmpty--;
			return E_OK;
		}
	}
}

/*******************************************************************************
 * 	Function name: 		TMU_DeInit
 *
 * 	Brief: 				De-initialize TMU module
 *
 * 	Details: 			De-initialize TMU module and
 *
 * 	Arguments:
 * 		-				timer_ch: the timer that the TMU works with
 *
 * 	Return:
 * 		-				ERROR_STATUS to report success or failure
 *******************************************************************************/
ERROR_STATUS TMU_DeInit(uint8 timer_ch)
{
	/* local declarations */
	ERROR_STATUS test = E_OK;
	if ( TMU_INIT != ge_modulestate )
	{
		return E_NOK;
	} 
	else
	{
		/* disable timer */
		test = Timer_Stop(timer_ch);
		test = Timer_DeInit(timer_ch);
		/* check if there were errors */
		if (E_NOK == test)
		{
			return E_NOK;
		} 
		else
		{
			/* no errors deinit module */
			ge_modulestate = TMU_DEINIT;
			return E_OK;
		}
	}
}

