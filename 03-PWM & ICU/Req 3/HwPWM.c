/******************************************************************************
 *
 * 	Module: Hardware PWM
 *
 *	File Name: HwPWM.c
 *
 * 	Description: PWM using hardware
 *
 * 	Authors: Mahmoud Saad
 * 	
 *	Created on: Sunday, 15-Dec-19 11:49:49 AM
 *
 *******************************************************************************/

/*- INCLUDES ----------------------------------------------*/
#include "HwPWM.h"
#define	F_CPU	(16000000UL)
/*- LOCAL MACROS ------------------------------------------*/
/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/
/*- GLOBAL STATIC VARIABLES -------------------------------*/
/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/
/*- APIs IMPLEMENTATION -----------------------------------*/


/**********************************************************************************************
 * 	Function name: HwPWM_Init
 *
 * 	Brief: 
 *
 * 	Details: 
 *
 * 	Arguments:
 * 		-
 *
 * 	Return:
 * 		-
 **********************************************************************************************/
void HwPWM_Init(void)
{
	/* init ports as output */
	gpioPinDirection(MOTOR_EN_1_GPIO, MOTOR_EN_1_BIT, HIGH);
	gpioPinDirection(MOTOR_EN_2_GPIO, MOTOR_EN_2_BIT, HIGH);
	/* 
	1-initialize timer 1 in phase correct, frequency correct mode.
	2-prescalar of clkI/O/64 16,000,000/64 = 250,000 ticks per second.
	3-A in high its to Clear OC1A/OC1B on compare match when
	up-counting. Set OC1A/OC1B on compare
	match when down counting.
	*/
	TCCR1 = 0xA011;
}

/**********************************************************************************************
 * 	Function name: HwPWM_ChangeWidth
 *
 * 	Brief: 
 *
 * 	Details: 
 *
 * 	Arguments:
 * 		-
 *
 * 	Return:
 * 		-
 **********************************************************************************************/
void HwPWM_ChangeWidth(uint8 duty, uint32 frequency)
{
	/*
	takes 250,000 ticks per second
	time of tick = 1/250,000 = 4us
	 */
	/* divide freq by 7 and multiply by 10/11 to change freq time to 1 =us */
	frequency = (F_CPU)/(2*frequency);
	ICR1 = frequency;
	OCR1A = duty*frequency/100;
	OCR1B = duty*frequency/100;
}

/*******************************************************************************
 *			Preprocessor Macros                                    
 *******************************************************************************/


/*******************************************************************************
 *			Functions Prototypes                                   
 *******************************************************************************/

/*******************************************************************************
*			Functions Definitions                                  
 *******************************************************************************/

/*******************************************************************************
 *			Private functions Prototypes                                   
 *******************************************************************************/
/*******************************************************************************
 *			Global variables
 *******************************************************************************/
/*******************************************************************************
 *			Static global variables
 *******************************************************************************/
/*******************************************************************************
 *			Extern Global variables
 *******************************************************************************/

/*******************************************************************************
 *			Enumerations and structures declarations                                  
 *******************************************************************************/
 


