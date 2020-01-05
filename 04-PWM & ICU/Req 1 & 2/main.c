/*
 * DIO_Timer.c
 *
 * Created: 12/3/2019 11:37:24 AM
 * Author : Amr
 */ 

#include "timers.h"
#include "interrupts.h"
#include "std_types.h"
#include "MotorDC.h"
#include "led.h"
#include "softwareDelay.h"
#include "SwICU.h"
#include "gpio.h"
#include <stdlib.h>

#define ULTRASONIC_MAX_DISTANCE						(400)
#define ULTRASONIC_MIN_DISTANCE						(2)
#define MAPPED_RANGE_MAX							(15)
#define MAPPED_RANGE_MIN							(0)
#define COEFFICIENT									( (ULTRASONIC_MAX_DISTANCE - ULTRASONIC_MIN_DISTANCE) / (MAPPED_RANGE_MAX - MAPPED_RANGE_MIN) )

#define DEBUG_WITH_LCD   (1)

#if DEBUG_WITH_LCD == 1
	#include "char_lcd.h"
	char str[4];
#endif

uint8 timer_ticks = 0;
uint32 echo_pulse_width = 0;
uint16 distance = 0;

int main(void)
{	
	uint8 mapped_value;		
	sei();
	Led_Init(LED_0);
	Led_Init(LED_1);
	Led_Init(LED_2);
	Led_Init(LED_3);
	
	#if DEBUG_WITH_LCD == 1
		LCD_init();
		LCD_clear();
	#endif
	
	SwICU_Init(SwICU_EdgeRisiging);
	
    while (1) 
    {	
		if ( TRIGGER_SEND == trigger_flag )
		{
			trigger_flag = TRIGGER_DONOT_SEND;
			SwICU_Read(&timer_ticks);
		}
			
		// calculate echo signal pulse width in micro seconds
		echo_pulse_width = ((timer_ticks + (ovf_counts * 256))*64);
		ovf_counts = 0;
		
		// calculate distance in cm
		distance = (echo_pulse_width * (double)0.0343) / 2;
		mapped_value = (distance / COEFFICIENT);
		gpioPortWrite(GPIOB, (mapped_value << 4));
		
		#if DEBUG_WITH_LCD == 1
			LCD_clear();
			LCD_goto_xy(0, 0);
			itoa(timer_ticks, str, 10);
			LCD_send_string(str);
			LCD_goto_xy(0, 1);
			itoa(distance, str, 10);
			LCD_send_string(str);
			SwDelay_ms(200);
		#endif
    }
}