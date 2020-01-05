/******************************************************************************
 *
 *	File Name: app.c
 *
 * 	Description: application file for payment project
 *
 * 	Authors: Mahmoud Saad
 *
 *	Created on: Dec 2, 2019
 *
 *******************************************************************************/


/*- INCLUDES ----------------------------------------------*/
#include "payment.h"
/*- LOCAL MACROS ------------------------------------------*/
/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/
/*- GLOBAL STATIC VARIABLES -------------------------------*/
/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/
/*- APIs IMPLEMENTATION -----------------------------------*/

int main (void)
{
	/* resolve Eclipse's terminal problem */
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	strcpy((char*)(gst_PaymentTransaction.transData.maxTransAmount), "5000.00");
	fillCardData(&gst_PaymentTransaction);
	fillTerminalData(&gst_PaymentTransaction);
	checkAmount(&gst_PaymentTransaction);
	saveTransactionIntoServer(&gst_PaymentTransaction);
	return 0;
}
