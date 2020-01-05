/*
 * car_PWM_req4.c
 *
 * Created: Monday, 16-Dec-19 2:08:11 PM
 * Author : Mahmoud Saad & Amr Mostafa
 */ 


#include "MotorDC.h"
#include "interrupts.h"
#include "SwICU.h"
#include "char_lcd.h"
#include "softwareDelay.h"
#include "HwPWM.h"
#include <stdlib.h>

// debug on lcd macro
#define DEBUG_WITH_LCD								(0U)

// ultrasonic macros
#define ULTRASONIC_MAX_DISTANCE						(400U)
#define ULTRASONIC_MIN_DISTANCE						(2U)
#define MAPPED_RANGE_MAX							(15U)
#define MAPPED_RANGE_MIN							(0U)
#define COEFFICIENT									( (ULTRASONIC_MAX_DISTANCE - ULTRASONIC_MIN_DISTANCE) / (MAPPED_RANGE_MAX - MAPPED_RANGE_MIN) )

// car macros for the speed
#define FORWARD_MAX_SPEED			(60U)
#define FORWARD_MIN_SPEED			(20U)
#define TURN_SPEED					(30U)
#define BACKWARD_SPEED				(35U)
#define STOP_SPEED					(0U)

// car macros for the running on different kits
#define		CAR_KIT_FORWARD			FORWARD
#define		CAR_KIT_BACKWARD		BACKWARD

// enum for car states
enum En_Car_States
{
	CAR_FORWARD_80,
	CAR_FORWARD_30,
	CAR_BACKWARD_30,
	CAR_TURN_RIGHT,
	CAR_STOP
};
enum En_Car_States car_states;

#if DEBUG_WITH_LCD == 1
	#include "char_lcd.h"
	char str[4];
#endif

uint8	g_timer_ticks = 0;
uint32	g_echo_pulse_width = 0;
uint16	g_distance = 0;
uint8	g_mapped_value;

volatile static uint8 g_car_state = CAR_STOP;

void ultrasonic_update(void)
{
	if ( TRIGGER_SEND == trigger_flag )
	{
		trigger_flag = TRIGGER_DONOT_SEND;
		SwICU_Read(&g_timer_ticks);
	}
	
	// calculate echo signal pulse width in micro seconds
	g_echo_pulse_width = ((g_timer_ticks + (ovf_counts * 256))*64);
	ovf_counts = 0;
	
	// calculate distance in cm
	g_distance = (g_echo_pulse_width * (double)0.0343) / 2;
	g_mapped_value = (g_distance / COEFFICIENT);
	gpioPortWrite(GPIOB, (g_mapped_value << 4));
	
	#if DEBUG_WITH_LCD == 1
		LCD_clear();
		LCD_goto_xy(0, 0);
		itoa(g_distance, str, 10);
		LCD_send_string(str);
		SwDelay_ms(200);
	#endif
}

void car_obstacle_avoidance(void)
{
	switch(g_car_state)
	{
		case CAR_STOP:
			MotorDC_Dir(MOT_1, STOP);
			MotorDC_Dir(MOT_2, STOP);
			MotorDC_Speed_HwPWM(STOP_SPEED);
			SwDelay_ms(100);
			if ( g_distance >= 30 )
			{
				g_car_state = CAR_TURN_RIGHT;
			}
			else if (  g_distance < 30  )
			{
				g_car_state = CAR_BACKWARD_30;
			}
			else 
			{
				
			}
		break;
		case CAR_FORWARD_30:
			MotorDC_Dir(MOT_1, CAR_KIT_FORWARD);
			MotorDC_Dir(MOT_2, CAR_KIT_FORWARD);
			MotorDC_Speed_HwPWM(FORWARD_MIN_SPEED);
			if ( g_distance > 50 )
			{
				g_car_state = CAR_FORWARD_80;
			}
			else if ( (g_distance <= 50) && (g_distance > 30) )
			{
				g_car_state = CAR_FORWARD_30;
			}
			else if ( g_distance <= 30 )
			{
				g_car_state = CAR_STOP;
			}		
			else
			{
				
			}
		break;
		case CAR_FORWARD_80:
			MotorDC_Dir(MOT_1, CAR_KIT_FORWARD);
			MotorDC_Dir(MOT_2, CAR_KIT_FORWARD);		
			MotorDC_Speed_HwPWM(FORWARD_MAX_SPEED);
			
			if ( g_distance > 50 )
			{
				g_car_state = CAR_FORWARD_80;
			}		
			else if ( (g_distance <= 50) && (g_distance > 30) )
			{
				g_car_state = CAR_FORWARD_30;
			}
			else if ( g_distance <= 30 )
			{
				g_car_state = CAR_STOP;
			}
			else 
			{
				
			}
		break;
		case CAR_BACKWARD_30:
			MotorDC_Dir(MOT_1, CAR_KIT_BACKWARD);
			MotorDC_Dir(MOT_2, CAR_KIT_BACKWARD);		
			MotorDC_Speed_HwPWM(BACKWARD_SPEED);
			if ( g_distance >= 30 )
			{
				g_car_state = CAR_STOP;
			}
			else if ( g_distance < 30 )
			{
				g_car_state = CAR_BACKWARD_30;
			}
		break;
		case CAR_TURN_RIGHT:
			MotorDC_Dir(MOT_1, CAR_KIT_BACKWARD);
			MotorDC_Dir(MOT_2, CAR_KIT_FORWARD);
			MotorDC_Speed_HwPWM(TURN_SPEED);
			SwDelay_ms(400);
			g_car_state = CAR_FORWARD_30;
		break;
		default:
		break;
	}
}


int main(void)
{
	
	sei();
	
	#if DEBUG_WITH_LCD == 1
		LCD_init();
		LCD_clear();
	#endif
	
	SwICU_Init(SwICU_EdgeRisiging);
	
	/* start the car */
    MotorDC_Init(MOT_1);
	MotorDC_Init(MOT_2);
	HwPWM_Init();
	g_car_state = CAR_FORWARD_30;
    while (1) 
    {
		ultrasonic_update();		
		car_obstacle_avoidance();
    }
}

