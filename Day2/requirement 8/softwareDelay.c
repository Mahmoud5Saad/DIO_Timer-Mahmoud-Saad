/******************************************************************************
 *
 *	File Name: softwareDelay.c
 *
 * 	Description: source file for software delay
 *
 * 	Authors: Mahmoud Saad
 *
 *	Created on: Dec 3, 2019
 *
 *******************************************************************************/


/*- INCLUDES ----------------------------------------------*/
#include "softwareDelay.h"
/*- LOCAL MACROS ------------------------------------------*/
/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/
/*- GLOBAL STATIC VARIABLES -------------------------------*/
/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/
/*- APIs IMPLEMENTATION -----------------------------------*/
/**
 * Description: this delay function operate in a polling manner
 * 				don't use it with RTOSs
 * @param n: the milli-seconds
 */
void SwDelay_ms(uint32 n)
{
	/* my board is working at 1 MHz */
	uint32 i;
	for (i=0; i<93*n;i++);
}
