/******************************************************************************
 *
 * 	Module: 		Small OS
 *
 *	File Name: 		sos.c
 *
 * 	Description: 	source file for small operating system
 *
 * 	Authors: 		Mahmoud Saad
 * 	
 *	Created on:		Monday, 20-Jan-20 12:17:01 PM
 *
 *******************************************************************************/

/*******************************************************************************
 *			INCLUDES                                    
 *******************************************************************************/
#include "sos.h"
#include "sos_Cfg.h"
#include "Timer.h" /* OS uses timer module */
/*******************************************************************************
 *			Preprocessor Macros                                    
 *******************************************************************************/
#define		SOS_TIME_OF_ONE_TICK				(64U)
#define		SOS_MILLISECONDS_IN_MICOSECONDS		(1000U)
/*******************************************************************************
 *			Enumerations and structures declarations                                  
 *******************************************************************************/
/* enum to hold module states */
typedef enum SOS_State
{
	SOS_DEINIT, SOS_INIT
}e_SOS_StateType;

typedef enum SOS_taskState
{
	SOS_TASK_WAITING, SOS_TASK_READY, SOS_TASK_RUNNING
}SOS_taskState;
/* structure to hold entries of each delay service */
typedef struct gstr_delayTaskBlockType
{
	gptrfu_TaskCall p_funcCall;
	uint32 u32_delay;
	uint32 u32_period;
	SOS_taskState	task_state;
	uint8 u8_priority;
}gstr_delayTaskBlockType;
/*******************************************************************************
 *			Functions Prototypes                                   
 *******************************************************************************/
/*******************************************************************************
 *			Private functions Prototypes                                   
 *******************************************************************************/
/*******************************************************************************
 *			Global variables
 *******************************************************************************/
/* array of OS tasks to be run */
gstr_delayTaskBlockType static gastr_SOSTasks[SOS_MAX_NUMBER_OF_TASKS];

/* global ticks of module */
volatile static uint32 gu32_SOSTicks = 0;

/* top of our delay list */
static uint32 gu32_currentFirstEmpty = 0;

/* global enum for module state */
e_SOS_StateType static ge_modulestate = SOS_DEINIT;

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
void SOS_ticksIncrement_cbk(void)
{
	gu32_SOSTicks++;
}

/*******************************************************************************
 * 	Function name: 		SOS_Init
 *
 * 	Brief: 				initialize SOS module
 *
 * 	Details: 			initialize SOS module and
 *
 * 	Arguments:
 * 		-				pointer to config_SOS_s structure which has 2 elements:
 *						-timer_ch timer channel
  *						-resoltuion_ticks resolution of smallest delay
 *
 * 	Return:
 * 		-				ERR_STATUS to report success or failure
 *******************************************************************************/
ERR_STATUS Sos_Init(gstr_config_SOS *cfgPtr)
{
	/* local declarations */
	uint8 timer_counts = 0;
	uint32 u32_index;
	ERR_STATUS retval = ERR_SUCCESS;
	if ( (NULL==cfgPtr) )
	{
		retval = SOS_BASE + ERR_NULL_PTR;
	}
	else if ( SOS_DEINIT!=ge_modulestate )
	{
		retval = SOS_BASE + ERR_ALREADY_INITIALIZED;
	}
	else
	{
		/* initialize module's arrays and variables */
		for (u32_index=0; u32_index<SOS_MAX_NUMBER_OF_TASKS; u32_index++)
		{
			gastr_SOSTasks[u32_index].p_funcCall = 0;
			gastr_SOSTasks[u32_index].u32_delay = 0;
			gastr_SOSTasks[u32_index].u32_period = 0;
			gastr_SOSTasks[u32_index].task_state = SOS_TASK_READY;
		}
		/* zero global ticks of module */
		gu32_SOSTicks = 0;
		gu32_currentFirstEmpty = 0;
		/*
		res is in milliseconds
		prescalar is TIMER_PRESCALER_64
		means one tick is 64 processor tick
		time of 1 tick = prescalar / F_CPU = 1024 / 16M = 64 micro seconds
		then divide resolution time on tick time and give the value to the timer to count.
		 */
		timer_counts = (SOS_MILLISECONDS_IN_MICOSECONDS*(cfgPtr->resoltuion_ticks))/SOS_TIME_OF_ONE_TICK ;
		Timer_cfg_s timer_SOS_init = 
		{
			cfgPtr->timer_ch,
			TIMER_MODE,
			TIMER_INTERRUPT_MODE,
			TIMER_PRESCALER_1024,
			&SOS_ticksIncrement_cbk
		};
		/* init timer and give it the value to count */
		retval = Timer_Init(&timer_SOS_init);
		if(retval == ERR_SUCCESS)
		{
			Timer_Start(cfgPtr->timer_ch,timer_counts);
			ge_modulestate = SOS_INIT;
		}
	}
	return retval;
}
/*******************************************************************************
 * 	Function name: 		SOS_Dispatcher
 *
 * 	Brief: 				dispatcher SOS module
 *
 * 	Details: 			dispatches delay services that are due in SOS module
 *						and notify the user using the pointers to functions 
 *						stored in its array
 *
 * 	Arguments:
 * 		-				void
 *
 * 	Return:
 * 		-				ERR_STATUS to report success or failure
 *******************************************************************************/
ERR_STATUS Sos_Run(void)
{
	gstr_delayTaskBlockType str_tempSwap;
	uint32 u32_index = 0;
	uint32 u32_sortIndex = 0;
	ERR_STATUS retval = ERR_SUCCESS;
	/* if there are unconsumed ticks, consume them */
	if (SOS_INIT!=ge_modulestate)
	{
		retval = SOS_BASE + ERR_NOT_INITIALIZED;
	} 
	else
	{
		/* system infinite loop */
		while (1)
		{
			while (gu32_SOSTicks > 0)
			{
				/* sort tasks based on priority */
				for (u32_index=0; u32_index<gu32_currentFirstEmpty; u32_index++)
				{
					for (u32_sortIndex=u32_index; u32_sortIndex<gu32_currentFirstEmpty; u32_sortIndex++)
					{
						if (gastr_SOSTasks[u32_index].u8_priority>gastr_SOSTasks[u32_sortIndex].u8_priority)
						{
							/* store in swap */
							str_tempSwap.p_funcCall = gastr_SOSTasks[u32_index].p_funcCall;
							str_tempSwap.task_state = gastr_SOSTasks[u32_index].task_state;
							str_tempSwap.u32_delay = gastr_SOSTasks[u32_index].u32_delay;
							str_tempSwap.u32_period = gastr_SOSTasks[u32_index].u32_period;
							str_tempSwap.u8_priority = gastr_SOSTasks[u32_index].u8_priority;
							/* swap to the start of the array */
							gastr_SOSTasks[u32_index].p_funcCall = gastr_SOSTasks[u32_sortIndex].p_funcCall;
							gastr_SOSTasks[u32_index].task_state = gastr_SOSTasks[u32_sortIndex].task_state;
							gastr_SOSTasks[u32_index].u32_delay = gastr_SOSTasks[u32_sortIndex].u32_delay;
							gastr_SOSTasks[u32_index].u32_period = gastr_SOSTasks[u32_sortIndex].u32_period;
							gastr_SOSTasks[u32_index].u8_priority = gastr_SOSTasks[u32_sortIndex].u8_priority;
							/* swap the opposite side */
							gastr_SOSTasks[u32_sortIndex].p_funcCall = str_tempSwap.p_funcCall;
							gastr_SOSTasks[u32_sortIndex].task_state = str_tempSwap.task_state;
							gastr_SOSTasks[u32_sortIndex].u32_delay = str_tempSwap.u32_delay;
							gastr_SOSTasks[u32_sortIndex].u32_period = str_tempSwap.u32_period;
							gastr_SOSTasks[u32_sortIndex].u8_priority = str_tempSwap.u8_priority;
						} 
						else {/*Do Nothing*/}
					}
				}
				u32_index = 0;
				/* consume the ticks */
				gu32_SOSTicks--;
				/* loop on filled part of the array of structures */
				for (u32_index=0; u32_index<gu32_currentFirstEmpty; u32_index++)
				{
					/* if pointer to function is not empty */
					if ( 0 != (gastr_SOSTasks[u32_index].p_funcCall) )
					{
						if (gastr_SOSTasks[u32_index].u32_delay <= 0) /* waiting is over */
						{
							/* task state */
							gastr_SOSTasks[u32_index].task_state = SOS_TASK_RUNNING;
							(*(gastr_SOSTasks[u32_index].p_funcCall))(); /* notify the user */
							(gastr_SOSTasks[u32_index].u32_delay) = (gastr_SOSTasks[u32_index].u32_period);
							/* task state */
							gastr_SOSTasks[u32_index].task_state = SOS_TASK_WAITING;
							if ( (gastr_SOSTasks[u32_index].u32_period) == 0) /* if one time task*/
							{
								/* remove from tasks */
								/* over write with later elements */
								for (u32_sortIndex=u32_index+1;u32_sortIndex<gu32_currentFirstEmpty;u32_sortIndex++)
								{
									gastr_SOSTasks[u32_sortIndex-1].p_funcCall = gastr_SOSTasks[u32_sortIndex].p_funcCall;
									gastr_SOSTasks[u32_sortIndex-1].task_state = gastr_SOSTasks[u32_sortIndex].task_state;
									gastr_SOSTasks[u32_sortIndex-1].u32_delay = gastr_SOSTasks[u32_sortIndex].u32_delay;
									gastr_SOSTasks[u32_sortIndex-1].u32_period = gastr_SOSTasks[u32_sortIndex].u32_period;
									gastr_SOSTasks[u32_sortIndex-1].u8_priority = gastr_SOSTasks[u32_sortIndex].u8_priority;
								}
								u32_index--;
								gu32_currentFirstEmpty--;
							}
						}
						else /* still waiting */
						{
							/* reduce delay period */
							(gastr_SOSTasks[u32_index].u32_delay)--;
						}
					}
				}
			}
			retval = ERR_SUCCESS;
		}
	}
	return retval;
}
/*******************************************************************************
 * 	Function name: 		Sos_Create_Task
 *
 * 	Brief: 				Starts a task in SOS
 *
 * 	Details: 			Starts a task to notify the user after a specific 
 *						delay period
 *
 * 	Arguments:
 * 		-				n_funcCall_Task pointer to func to call the task
 * 		-				n_delay the period to notify after in the first time
 * 		-				n_period  the period to notify after in subsequest times
 *							-if 0, it means one time task, not periodic
 *						n_priority priority of task
 *
 * 	Return:
 * 		-				the u32_index of the task created
 *******************************************************************************/
uint32 Sos_Create_Task(gptrfu_TaskCall n_funcCall_Task,uint32 n_delay, uint32 n_period, uint8 n_priority)
{
	/* u32_index of delay service */
	uint32 u32_index = 0;
	if ( NULL == n_funcCall_Task)
	{
		return E_NOK;
	} 
	else
	{
		/* add delay service */
		gastr_SOSTasks[u32_index].task_state = SOS_TASK_WAITING;
		gastr_SOSTasks[gu32_currentFirstEmpty].p_funcCall = n_funcCall_Task;
		gastr_SOSTasks[gu32_currentFirstEmpty].u32_delay = n_delay;
		gastr_SOSTasks[gu32_currentFirstEmpty].u32_period = n_period;
		gastr_SOSTasks[gu32_currentFirstEmpty].u8_priority = n_priority;
		u32_index = gu32_currentFirstEmpty;
		gu32_currentFirstEmpty++;
		/* return u32_index to user so that he can use later for reference */
		return u32_index;
	}
}
/*******************************************************************************
 * 	Function name: 		Sos_Delete_Task
 *
 * 	Brief: 				Stops a delay service in SOS
 *
 * 	Details: 			stops a task to notify the user after a specific 
 *						delay period
 *
 * 	Arguments:
 * 		-				deleteByu32_index: u32_index of task to be deleted

 *
 * 	Return:
 * 		-				return success or failure
 *******************************************************************************/
ERR_STATUS Sos_Delete_Task(uint32 deleteByu32_index)
{
	ERR_STATUS retval = ERR_SUCCESS;
	if (deleteByu32_index>gu32_currentFirstEmpty)
	{
		/* return error if wrong u32_index */
		retval = SOS_BASE + ERR_RESOURCE_NOT_FOUND;
	} 
	else
	{
		/* remove task from array */
		gastr_SOSTasks[deleteByu32_index].p_funcCall = NULL;
		gastr_SOSTasks[deleteByu32_index].u32_period = 0;
		gastr_SOSTasks[deleteByu32_index].u32_delay = 0;
		if (deleteByu32_index==(gu32_currentFirstEmpty-1))
		{
			/* if last task was deleted return */
			gu32_currentFirstEmpty--;
			retval = ERR_SUCCESS;
		} 
		else
		{
			/* if not last task, copy last task in its place */
			gastr_SOSTasks[deleteByu32_index].p_funcCall = gastr_SOSTasks[gu32_currentFirstEmpty-1].p_funcCall;
			gastr_SOSTasks[deleteByu32_index].u32_delay = gastr_SOSTasks[gu32_currentFirstEmpty-1].u32_delay;
			gastr_SOSTasks[deleteByu32_index].u32_period = gastr_SOSTasks[gu32_currentFirstEmpty-1].u32_period;
			/* delete the copied task */
			gastr_SOSTasks[gu32_currentFirstEmpty-1].p_funcCall = 0;
			gastr_SOSTasks[gu32_currentFirstEmpty-1].u32_delay = 0;
			gastr_SOSTasks[gu32_currentFirstEmpty-1].u32_period = 0;
			/* decrement the top of tasks*/
			gu32_currentFirstEmpty--;
			retval = ERR_SUCCESS;
		}
	}
	return retval;
}

