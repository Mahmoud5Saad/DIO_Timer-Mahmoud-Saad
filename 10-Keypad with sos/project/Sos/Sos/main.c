/*
 * Sos.c
 *
 * Created: Monday, 20-Jan-20 12:16:40 PM
 * Author : Mahmoud Saad
 */ 

/*******************************************************************************
 *			INCLUDES                                    
 *******************************************************************************/
#include "sos.h"
#include "DIO.h"
#include "Timer.h" /* for ch_0 macro */
#include "keypad.h"
#include "lcd.h"
/*******************************************************************************
 *			Preprocessor Macros                                    
 *******************************************************************************/
#define NUMBER_OF_BUTTONS (9U)
/*******************************************************************************
 *			Static global variables
 *******************************************************************************/
uint8 u8_port_KeypadLCD = 0;
 /*******************************************************************************
 *			Functions Definitions                                  
 *******************************************************************************/
void task1(void)
{
	DIO_Toggle(GPIOB, BIT4);
}

void task2(void)
{
	DIO_Toggle(GPIOB, BIT5);
}

void LCD_displaycharacter_keypad(void)
{
	LCD_integerToString(u8_port_KeypadLCD);
}

void checkKeypad(void)
{
	uint8 index = 0;
	/* static variable to save click */
	Keypad_buttonState static ea_buttonLastStatus[9] = 
	{
		KEYPAD_RELEASED,
		KEYPAD_RELEASED,
		KEYPAD_RELEASED,
		KEYPAD_RELEASED,
		KEYPAD_RELEASED,
		KEYPAD_RELEASED,
		KEYPAD_RELEASED,
		KEYPAD_RELEASED,
		KEYPAD_RELEASED
	};
	Keypad_buttonState ea_buttonCurrentStatus[9];
	for (index=0; index<NUMBER_OF_BUTTONS; index++)
	{
		ea_buttonCurrentStatus[index] = KEYPAD_getPressedKey(index);
		if ((KEYPAD_RELEASED==ea_buttonCurrentStatus[index])&&(KEYPAD_PRESSED==ea_buttonLastStatus[index]))
		{
			ea_buttonLastStatus[index] = KEYPAD_RELEASED;
			u8_port_KeypadLCD = index + 1;
			Sos_Create_Task(&LCD_displaycharacter_keypad, 0, 0, 3);
		} 
		else if (KEYPAD_PRESSED==ea_buttonCurrentStatus[index])
		{
			ea_buttonLastStatus[index] = KEYPAD_PRESSED;
		} else {/*Do Nothing*/}
	}
}
int main(void)
{
	sei();
	LCD_init();
	Keypad_init();
	DIO_Cfg_s test1 = { GPIOB, FULL_PORT, OUTPUT };
	DIO_init(&test1);
	gstr_config_SOS SOS_initStruct = { TIMER_CH0, 1 };
	Sos_Init(&SOS_initStruct);
	Sos_Create_Task(&task1, 1000, 1000, 5);
	Sos_Create_Task(&task2, 500, 1000, 5);
	Sos_Create_Task(&Keypad_Update, 30, 30, 1);
	Sos_Create_Task(&checkKeypad, 200, 200, 2);
	Sos_Run();
}

