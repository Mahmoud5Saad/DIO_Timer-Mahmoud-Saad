/******************************************************************************
 *
 * 	Module: 		SOS
 *
 *	File Name: 		sos.h
 *
 * 	Description: 	header file for sos
 *
 * 	Authors: 		Mahmoud Saad
 * 	
 *	Created on:		Monday, 20-Jan-20 1:23:28 PM
 *
 *******************************************************************************/

#ifndef SOS_H_
#define SOS_H_

/*******************************************************************************
 *			INCLUDES                                    
 *******************************************************************************/
#include "std_types.h"
#include "interrupt.h"
#include "Processor.h"
#include "common_retval.h"
/*******************************************************************************
 *			Preprocessor Macros                                    
 *******************************************************************************/
/*******************************************************************************
 *				Enumerations and structures declarations                                  
 *******************************************************************************/
typedef struct gstr_config_SOS
{
	uint8	timer_ch;
	uint32	resoltuion_ticks;
}gstr_config_SOS;

/* type of pointer to function used for user task call */
typedef void (*gptrfu_TaskCall)(void);
/*******************************************************************************
 *				Functions Prototypes                                   
 *******************************************************************************/
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
 * 		-				ERROR_STATUS to report success or failure
 *******************************************************************************/
extern ERR_STATUS Sos_Init(gstr_config_SOS *cfgPtr);
/*******************************************************************************
 * 	Function name: 		Sos_Run
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
 * 		-				ERROR_STATUS to report success or failure
 *******************************************************************************/
extern ERR_STATUS Sos_Run(void);
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
extern uint32 Sos_Create_Task(gptrfu_TaskCall n_funcCall_Task,uint32 n_delay, uint32 n_period, uint8 n_priority);
/*******************************************************************************
 * 	Function name: 		Sos_Delete_Task
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
extern ERR_STATUS Sos_Delete_Task(uint32 deleteByIndex);


#endif /* SOS_H_ */