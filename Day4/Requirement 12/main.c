/*
 * interrupts.c
 *
 * Created: Monday, 09-Dec-19 10:38:47 AM
 * Author : Mahmoud Saad
 */ 

#include "timers.h"
#include "MotorDC.h"

#define ROTATE_RIGHT 2000

int main(void)
{
    timer1Init(T1_NORMAL_MODE, T1_OC0_DIS, T1_PRESCALER_NO, 0, 0, 0, 0, T1_POLLING);
	
	MotorDC_Init(MOT_1);
	MotorDC_Init(MOT_2);
	
	uint32 j;
	uint32 count;
	
	/** How the code works:
	 * when timer1 swPWM function takes a frequency of 1KHz, it outputs 1 pulse for 1 milli second,
	 * so, each 10+ increase in the speed, we count for 500 to get 0.5 sec PWM on the current speed.
	 * repeat this within the increasing outer for loop will results in 0>100 acceleration within 5 seconds.
	 * and vice-versa for the decreasing outer loop
	 */
	
	
	/* increase the speed by 10% every 0.5*/
	for(j=0; j<10; j++)
	{
		/* stay on the current speed for 0.5 second */
		for (count=0; count < 500; count++)
		{
			MotorDC_Speed_PollingWithT0(((j+1)*10)-1);
			MotorDC_Dir(MOT_1, FORWARD);
			MotorDC_Dir(MOT_2, FORWARD);
		}
	}
	
	/* decrease the speed by 10% every 0.5*/
	for(; j>0; j--)
	{
		/* stay on the current speed for 0.5 second */
		for (count=0; count < 500; count++)
		{
			MotorDC_Speed_PollingWithT0(((j)*10)-1);
			MotorDC_Dir(MOT_1, FORWARD);
			MotorDC_Dir(MOT_2, FORWARD);
		}
	}
	
	/* rotate right 90 degree */
	for (count=0; count < ROTATE_RIGHT; count++)
	{
		MotorDC_Speed_PollingWithT0(50);
		MotorDC_Dir(MOT_1, STOP);
		MotorDC_Dir(MOT_2, FORWARD);
	}
	
	/* stop the car */
	for (count=0; count < 1000; count++)
	{
		MotorDC_Speed_PollingWithT0(80);
		MotorDC_Dir(MOT_1, STOP);
		MotorDC_Dir(MOT_2, STOP);
	}
    while (1) 
    {
		
    }
}

