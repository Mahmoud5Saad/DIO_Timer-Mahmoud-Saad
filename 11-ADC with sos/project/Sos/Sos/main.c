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
#include "adc.h"
/*******************************************************************************
 *			Preprocessor Macros                                    
 *******************************************************************************/
#define NUMBER_OF_BUTTONS (9U)
/*******************************************************************************
 *			Static global variables
 *******************************************************************************/
uint8 u8_port_KeypadLCD = 0;
uint16 gu16_ADCRead = 0;
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
void readADC(void)
{
	ADC_Start_Polling(5,&gu16_ADCRead);
}

void displayTempLCD(void)
{
	gu16_ADCRead = gu16_ADCRead*4.88/10;
	/* clear number */
	LCD_goToRowColumn(0,7);
	LCD_displayString("    ");
	/* display new number */
	LCD_goToRowColumn(0,7);
	LCD_integerToString(gu16_ADCRead);
}

int main(void)
{
	sei();
	str_ADC_cfg_t gstr_ADC_init =
	{
		ADC_POLLING,
		ADC0_SINGLE,
		ADC_PRESCALER_4,
		ANALOGCOMP,
		NULL
	};
	ADC_Init(&gstr_ADC_init);
	LCD_init();
	Keypad_init();
	LCD_displayString("Temp: ");
	DIO_Cfg_s test1 = { GPIOB, FULL_PORT, OUTPUT };
	DIO_init(&test1);
	gstr_config_SOS SOS_initStruct = { TIMER_CH0, 1 };
	Sos_Init(&SOS_initStruct);
	Sos_Create_Task(&task1, 1000, 1000, 5);
	Sos_Create_Task(&task2, 500, 1000, 5);
	Sos_Create_Task(&readADC, 1000, 1000, 1);
	Sos_Create_Task(&displayTempLCD, 1000, 1000, 2);
	Sos_Run();
}

