/*
 * pushButton.c
 *
 * Created: 12/5/2019 5:40:14 PM
 *  Author: Amr
 */ 
#include "pushButton.h"
#include "gpio.h"
#include "pushButtonConfig.h"
#include "timers.h"

#define BTN_SAMPLES					2
#define BTN_UPDATE_PERIOD			(10)			// update buttons states every Xms

/* struct for saving button samples and current state */
struct ST_ButtonInfo
{
	uint8 button_samles_arr[BTN_SAMPLES];
	En_buttonStatus_t button_state;
};

static struct ST_ButtonInfo buttons_info[BTN_MAX_NUM];
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
		gpioPinDirection(BTN_0_GPIO, BTN_0_BIT, INPUT);							// make pin input
		#if BTN_PRESSED_LEVEL == 0
			gpioPinWrite(BTN_0_GPIO, BTN_0_BIT, 1);								// activate pull up resistor if pressed level is 0
		#endif
		buttons_info[BTN_0].button_samles_arr[0] = BTN_RELEASED_LEVEL;			// initialize sample 0 as released level
		buttons_info[BTN_0].button_samles_arr[1] = BTN_RELEASED_LEVEL;			// initialize sample 1 as pressed level
		buttons_info[BTN_0].button_state = Released;							// initialize button state as released
		break;
	
	case BTN_1:
		gpioPinDirection(BTN_1_GPIO, BTN_1_BIT, INPUT);							// make pin input
		#if BTN_PRESSED_LEVEL == 0
		gpioPinWrite(BTN_1_GPIO, BTN_1_BIT, 1);									// activate pull up resistor if pressed level is 0
		#endif
		buttons_info[BTN_1].button_samles_arr[0] = BTN_RELEASED_LEVEL;			// initialize sample 0 as released level
		buttons_info[BTN_1].button_samles_arr[1] = BTN_RELEASED_LEVEL;			// initialize sample 1 as pressed level
		buttons_info[BTN_1].button_state = Released;							// initialize button state as released
		break;
	
	case BTN_2:
		gpioPinDirection(BTN_2_GPIO, BTN_2_BIT, INPUT);							// make pin input
		#if BTN_PRESSED_LEVEL == 0
		gpioPinWrite(BTN_2_GPIO, BTN_2_BIT, 1);									// activate pull up resistor if pressed level is 0
		#endif
		buttons_info[BTN_2].button_samles_arr[0] = BTN_RELEASED_LEVEL;			// initialize sample 0 as released level
		buttons_info[BTN_2].button_samles_arr[1] = BTN_RELEASED_LEVEL;			// initialize sample 1 as pressed level
		buttons_info[BTN_2].button_state = Released;							// initialize button state as released
		break;
	
	case BTN_3:
		gpioPinDirection(BTN_3_GPIO, BTN_3_BIT, INPUT);							// make pin input
		#if BTN_PRESSED_LEVEL == 0
		gpioPinWrite(BTN_3_GPIO, BTN_3_BIT, 1);									// activate pull up resistor if pressed level is 0
		#endif
		buttons_info[BTN_3].button_samles_arr[0] = BTN_RELEASED_LEVEL;			// initialize sample 0 as released level
		buttons_info[BTN_3].button_samles_arr[1] = BTN_RELEASED_LEVEL;			// initialize sample 1 as pressed level
		buttons_info[BTN_3].button_state = Released;							// initialize button state as released
		break;
		
	default:
		break;
	}
}
/**
 * Description: read all BTN_x (where x 0, 1, 2, 3) states and store it in the program
 *
 * @note : this function must be called in the system tick handler or in the super loop handler
 */
void pushButton_Update(void)
{
	uint8 i;
	
	/* read samples and update states every 10 ms */
	timer0Delay_ms(BTN_UPDATE_PERIOD);
	
	for (i=0; i<BTN_MAX_NUM; i++)
	{
		/* first: update buttons inputs */
		buttons_info[i].button_samles_arr[0] = buttons_info[i].button_samles_arr[1];
		
		if (i == 0)
		{
			buttons_info[i].button_samles_arr[1] = gpioPinRead(BTN_0_GPIO, BTN_0_BIT);
			
		}
		else if (i == 1)
		{
			buttons_info[i].button_samles_arr[1] = gpioPinRead(BTN_1_GPIO, BTN_0_BIT);
		}
		else if (i == 2)
		{
			buttons_info[i].button_samles_arr[1] = gpioPinRead(BTN_2_GPIO, BTN_0_BIT);
		}
		else if (i == 3)
		{
			buttons_info[i].button_samles_arr[1] = gpioPinRead(BTN_3_GPIO, BTN_0_BIT);
		}
		else
		{
			
		}
		
		/* second: update buttons states */
		switch (buttons_info[i].button_state)
		{
		case Released:
			if (buttons_info[i].button_samles_arr[0] == BTN_PRESSED_LEVEL && buttons_info[i].button_samles_arr[1] == BTN_PRESSED_LEVEL)
			{
				buttons_info[i].button_state = Prepressed;
			}
			else
			{
				
			}
			break;
		
		case Prepressed:
			if (buttons_info[i].button_samles_arr[1] == BTN_PRESSED_LEVEL)
			{
				buttons_info[i].button_state = Pressed;
			}
			else
			{
				
			}
			break;
		
		case Pressed:
			if (buttons_info[i].button_samles_arr[0] == BTN_RELEASED_LEVEL && buttons_info[i].button_samles_arr[1] == BTN_RELEASED_LEVEL)
			{
				buttons_info[i].button_state = Prereleased;
			}
			else
			{
				
			}
			break;
			
		case Prereleased:
			if (buttons_info[i].button_samles_arr[1] == BTN_RELEASED_LEVEL)
			{
				buttons_info[i].button_state = Released;
			}
			else
			{
				
			}
			break;
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
	return buttons_info[btn_id].button_state;
}