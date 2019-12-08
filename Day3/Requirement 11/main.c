/*
 * timer1.c
 *
 * Created: Sunday, 08-Dec-19 12:08:43 PM
 * Author : Mahmoud Saad
 */ 

#include "led.h"
#include "timers.h"


#define MOTOR_OUT_1A_GPIO	(GPIOD)
#define MOTOR_OUT_1A_BIT	(BIT1)

int main(void)
{
	timer1Init(T1_NORMAL_MODE, T1_OC0_DIS, T1_PRESCALER_256, 0, /*(655 * dutyCycle)*/ 0X7FFF, 0, 0, T1_POLLING);
	Led_Init(LED_1);
	Led_Off(LED_1);
	gpioPinDirection(MOTOR_OUT_1A_GPIO, MOTOR_OUT_1A_BIT, HIGH);
    /* Replace with your application code */
    while (1) 
    {
		timer1SwPWM(0,0);
    }
}

