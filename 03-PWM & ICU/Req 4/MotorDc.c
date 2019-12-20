/*
 * MotorDC.c
 *
 * Created: 12/14/2019 9:59:40 AM
 *  Author: Amr
 */ 

/*- INCLUDES ----------------------------------------------*/
#include "MotorDC.h"
#include "gpio.h"
#include "timers.h"
#include "HwPWM.h"

/*- LOCAL MACROS ------------------------------------------*/
#define		ONE_KHZ	(1000U)

/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/
/*- GLOBAL STATIC VARIABLES -------------------------------*/
/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/
/*- APIs IMPLEMENTATION -----------------------------------*/

/**
 * Description: 
 * @param
 */
void MotorDC_Init(En_motorType_t MOT_x)
{	
	switch (MOT_x)
	{
		/* motor 1 */
	case MOT_1:
		gpioPinDirection(MOTOR_EN_1_GPIO, MOTOR_EN_1_BIT, OUTPUT);
		gpioPinDirection(MOTOR_OUT_1A_GPIO, MOTOR_OUT_1A_BIT,OUTPUT);
		gpioPinDirection(MOTOR_OUT_1B_GPIO, MOTOR_OUT_1B_BIT,OUTPUT);
		MotorDC_Dir(MOT_1, STOP);
		break;
	case MOT_2:
		/* motor2 */
		gpioPinDirection(MOTOR_EN_2_GPIO, MOTOR_EN_2_BIT, OUTPUT);
		gpioPinDirection(MOTOR_OUT_2A_GPIO, MOTOR_OUT_2A_BIT, OUTPUT);
		gpioPinDirection(MOTOR_OUT_2B_GPIO, MOTOR_OUT_2B_BIT, OUTPUT);
		MotorDC_Dir(MOT_2, STOP);
		break;
	default:
		break;
	}	
}

/**
 * Description: 
 * @param 
 * @param 
 */
void MotorDC_Dir(En_motorType_t MOT_x, En_motorDir_t dir)
{	
	switch (MOT_x)
	{
	case MOT_1:
		switch(dir)
		{
			case STOP:
			gpioPinWrite(MOTOR_OUT_1A_GPIO, MOTOR_OUT_1A_BIT, LOW);
			gpioPinWrite(MOTOR_OUT_1B_GPIO, MOTOR_OUT_1B_BIT, LOW);
			break;
			
			case FORWARD:
			gpioPinWrite(MOTOR_OUT_1A_GPIO, MOTOR_OUT_1A_BIT, HIGH);
			gpioPinWrite(MOTOR_OUT_1B_GPIO, MOTOR_OUT_1B_BIT, LOW);
			break;
			
			case BACKWARD:
			gpioPinWrite(MOTOR_OUT_1A_GPIO, MOTOR_OUT_1A_BIT, LOW);
			gpioPinWrite(MOTOR_OUT_1B_GPIO, MOTOR_OUT_1B_BIT, HIGH);
			break;
			
			default:
			break;
		}
		break;
	
	case MOT_2:
		switch(dir)
		{
			case STOP:
			gpioPinWrite(MOTOR_OUT_2A_GPIO, MOTOR_OUT_2A_BIT, LOW);
			gpioPinWrite(MOTOR_OUT_2B_GPIO, MOTOR_OUT_2B_BIT, LOW);
			break;
		
			case FORWARD:
			gpioPinWrite(MOTOR_OUT_2A_GPIO, MOTOR_OUT_2A_BIT, HIGH);
			gpioPinWrite(MOTOR_OUT_2B_GPIO, MOTOR_OUT_2B_BIT, LOW);
			break;
		
			case BACKWARD:
			gpioPinWrite(MOTOR_OUT_2A_GPIO, MOTOR_OUT_2A_BIT, LOW);
			gpioPinWrite(MOTOR_OUT_2B_GPIO, MOTOR_OUT_2B_BIT, HIGH);
			break;
		
			default:
			break;
		}
	break;
	}	
}

/**
 * Description: 
 * @param 
 */
void MotorDC_Speed_PollingWithT0(uint8 speed)
{
	timer1SwPWM(speed, 1);			// running motors on freq of 1KHz
}

/**
 * Description: set the port value (which is PORT register)
 * @param 
 */
void MotorDC_Speed_HwPWM(uint8 speed)
{
	HwPWM_ChangeWidth(speed, ONE_KHZ);
}