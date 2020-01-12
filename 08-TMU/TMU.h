/*
 * TMU.h
 *
 * Created: Saturday, 11-Jan-20 5:54:40 PM
 *  Author: Mahmoud Saad
 */ 


#ifndef TMU_H_
#define TMU_H_

/*******************************************************************************
 *			INCLUDES                                    
 *******************************************************************************/
#include "std_types.h"
#include "interrupt.h"
#include "Processor.h"
/*******************************************************************************
 *			Preprocessor Macros                                    
 *******************************************************************************/
/*******************************************************************************
 *			Enumerations and structures declarations                                  
 *******************************************************************************/
typedef struct config_TMU_s
{
	uint8	timer_ch;
	uint32	resoltuion_ticks;
}config_TMU_s;


/*******************************************************************************
 *			Functions Prototypes                                   
 *******************************************************************************/
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
ERROR_STATUS TMU_Init(config_TMU_s *cfgPtr);
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
ERROR_STATUS TMU_Dispatcher();
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
uint32 TMU_Start_time(void (*n_Cbk_delayTask)(void),uint32 n_delay, uint32 n_period);
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
ERROR_STATUS TMU_Stop_Time(uint32 deleteByIndex);
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
ERROR_STATUS TMU_DeInit(uint8 timer_ch);


#endif /* TMU_H_ */