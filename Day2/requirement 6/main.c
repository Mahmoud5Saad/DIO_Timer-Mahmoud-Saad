/******************************************************************************
 *
 *	File Name: app.c
 *
 * 	Description: source file for application
 *
 * 	Authors: Mahmoud Saad
 *
 *	Created on: Dec 4, 2019
 *
 *******************************************************************************/


#define F_CPU 1000000UL

/*- INCLUDES ----------------------------------------------*/
#include <util/delay.h>
#include "gpio.h"
#include "softwareDelay.h"
#include "led.h"
#include "pushButton.h"
/*- LOCAL MACROS ------------------------------------------*/
#define ONE_SECOND 33
#define TWO_SECONDS 66
#define DEBOUNCE 30
/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/
/*- GLOBAL STATIC VARIABLES -------------------------------*/
/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/
/*- APIs IMPLEMENTATION -----------------------------------*/

uint32 g_countOneSecond = 0;
En_buttonStatus_t g_oldPress = Released;
uint32 g_totalDelay = ONE_SECOND;
int main(void)
{
	pushButton_Init(BTN_1);
	Led_Init(LED_1);
	Led_Off(LED_1);
	for(;;)
	{
		SwDelay_ms(DEBOUNCE);
		pushButton_Update();
		if ( (pushButton_GetStatus(BTN_1) == Pressed) || (g_countOneSecond) || (g_oldPress == Pressed) )
		{
			Led_On(LED_1);
			g_oldPress = Pressed;
			if(g_countOneSecond == 0)
			{
				g_totalDelay = ONE_SECOND;
			}
			if ( (pushButton_GetStatus(BTN_1) == Released) || (g_countOneSecond) )
			{
				g_oldPress = Released;
				g_countOneSecond++;
				Led_On(LED_1);
				if ( (pushButton_GetStatus(BTN_1) == Pressed) && g_countOneSecond < ONE_SECOND  )
				{
					g_totalDelay = TWO_SECONDS;
				}
				{
					
				}
				if (g_countOneSecond >= g_totalDelay)
				{
					g_countOneSecond = 0;
					Led_Off(LED_1);
				}
			}
		}
	}
}
