/*
 * icu.c
 *
 * Created: Sunday, 15-Dec-19 9:35:30 AM
 * Author : Mahmoud Saad
 */ 

#include "HwPWM.h"
#include "interrupts.h"
#include "MotorDC.h"
#include "timers.h"

volatile uint32 g_ticks = 0;
uint8 g_duty = 0;
volatile uint32 g_half_seconds = 0;
int main(void)
{
	sei();
	HwPWM_Init();
	HwPWM_ChangeWidth(g_duty,1000);
	MotorDC_Init(MOT_1);
	MotorDC_Init(MOT_2);
	MotorDC_Dir(MOT_1, FORWARD);
	MotorDC_Dir(MOT_2, FORWARD);
	
    //timer2Init(T2_NORMAL_MODE, T2_OC0_DIS, T2_PRESCALER_1024, 0, 0, 0, T2_INTERRUPT_NORMAL);
	timer2Init(T2_NORMAL_MODE, T2_OC0_DIS, T2_PRESCALER_1024, 0, 0, 0, T2_INTERRUPT_NORMAL);
	gpioPinDirection(GPIOB, BIT2, HIGH);
	timer0Start();
    while (1) 
    {
		if ( g_ticks >= 3 )
		{
			if ( g_half_seconds <= 100 )
			{
				g_duty += 1;
				HwPWM_ChangeWidth(g_duty,1000);
			}
			else if ( g_half_seconds > 100 && g_half_seconds <= 200)
			{
				g_duty -= 1;
				HwPWM_ChangeWidth(g_duty,1000);
			}
			else if ( g_half_seconds > 200 && g_half_seconds <= 210)
			{
				HwPWM_ChangeWidth(50,1000);
				MotorDC_Dir(MOT_1, FORWARD);
				MotorDC_Dir(MOT_2, BACKWARD);
			}
			else
			{
				MotorDC_Dir(MOT_1, STOP);
				MotorDC_Dir(MOT_2, STOP);
			}
			g_ticks = 0;
			g_half_seconds++;
		}
    }
}

ISR(TIMER2_OVF_VECTOR)
{
	g_ticks++;
}