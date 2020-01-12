/******************************************************************************
 *
 * 	Module: 		TMU
 *
 *	File Name: 		TMU_Cbk.h
 *
 * 	Description: 	call back functions of TMU module for Timer management unit
 *
 * 	Authors: 		Mahmoud Saad
 * 	
 *	Created on:		Saturday, 11-Jan-20 3:43:18 PM
 *
 *******************************************************************************/

#ifndef TMU_CBK_H_
#define TMU_CBK_H_

/*******************************************************************************
 *			Functions Prototypes                                   
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
void delayTicks_Increment(void);



#endif /* TMU_CBK_H_ */