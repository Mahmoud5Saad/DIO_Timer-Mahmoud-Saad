/*
* pushButton.c
*
* Created: 12/7/2019 10:20:58 AM
*  Author: Mahmoud Saad
*/


#include "pushButton.h"
#include "gpio.h"

#define PRESSED			1U
#define NOT_PRESSED		0U

static En_buttonStatus_t g_button0 = Released;
static En_buttonStatus_t g_button1 = Released;

/**
* Description: Initialize the BTN_x Pin state (where x 0, 1, 2, 3) to Input
* @param btn_id: The btn to be initialized and it takes
* 				  one of the enum (En_buttonId) parameters
*
*/
void pushButton_Init(En_buttonId btn_id)
{
	switch (btn_id)
	{
		case BTN_0:
		gpioPinDirection(BTN_0_GPIO, BTN_0_BIT, INPUT);
		break;
		
		case BTN_1:
		gpioPinDirection(BTN_1_GPIO,BTN_1_BIT, INPUT);
		break;
		
		default:
		break;
	}
}
/**
* Description: read all BTN_x (where x 0, 1, 2, 3) states and store it in the program
*
* @note : this function must be called in the system tick hundler or in the super loop hundler
*/
void pushButton_Update(void)
{
	/* button 0 update */
	if ( gpioPinRead(BTN_0_GPIO, BTN_0_BIT)  )
	{
		if ( Prepressed == g_button0)
		{
			g_button0 = Pressed;
		}
		else if ( Released == g_button0 )
		{
			g_button0 = Prepressed;
		}
	}
	else
	{
		if ( Prereleased == g_button0)
		{
			g_button0 = Released;
		}
		else if ( Pressed == g_button0 )
		{
			g_button0 = Prereleased;
		}
	}
	/* button 1 update */
	if ( PRESSED == gpioPinRead(BTN_1_GPIO, BTN_1_BIT)  )
	{
		if ( Prepressed == g_button1)
		{
			g_button1 = Pressed;
		}
		else if ( Released == g_button1 )
		{
			g_button1 = Prepressed;
		}
	}
	else
	{
		if ( Prereleased == g_button1)
		{
			g_button1 = Released;
		}
		else if ( Pressed == g_button1 )
		{
			g_button1 = Prereleased;
		}
	}
}

/**
* Description: read BTN_x (where x 0, 1, 2, 3) state which is stored in the program
* 				in the update function
*
*/
En_buttonStatus_t pushButton_GetStatus(En_buttonId btn_id)
{
	En_buttonStatus_t buttonState = 0xFF;
	switch (btn_id)
	{
		case BTN_0:
		buttonState = g_button0;
		break;
		
		case BTN_1:
		buttonState = g_button1;
		break;
		
		default:
		break;
	}
	return buttonState;
}

