/*
 * DIO_Timer.c
 *
 * Created: 12/3/2019 11:37:24 AM
 * Author : Amr
 */ 

#define F_CPU	16000000UL
#include "timers.h"
#include "interrupts.h"
#include "std_types.h"
#include "MotorDC.h"

/* macro definitions to run the PWM on a frequency = 1KHz always */
#define PWM_FREQ					(1000UL)
#define PWM_TCNT_TICKS				(F_CPU / (PWM_FREQ * TIMER1_PRESCALER))
#define PWM_TCNT_START				(0xFFFF - PWM_TCNT_TICKS)

volatile uint8 dutyCycle = 0;
volatile uint32 ocr_ticks = 0;
volatile uint16 ocr_start = 0;
volatile uint16 state_change_millis_counter = 0;
volatile uint16 speed_change_millis_counter = 0;

/* car states enum */
enum EN_CarStates{
	CAR_ACCELERATION,
	CAR_DECELERATION,
	CAR_TURN_RIGHT,
	CAR_ENGINE_OFF
};

/* start the car with the "CAR_ACCELERATION" state */	
enum EN_CarStates car_state = CAR_ACCELERATION;
	
void car_control(void)
{
	switch (car_state)
	{
		case CAR_ACCELERATION:
			if ( speed_change_millis_counter >= 50 )
			{
				/* increase duty cycle by 1% every 50 milli seconds & make counter = 0 */
				dutyCycle += 1;
				speed_change_millis_counter = 0;
			}
			MotorDC_Dir(MOT_1, FORWARD);
			MotorDC_Dir(MOT_2, FORWARD);
			
			if ( state_change_millis_counter >= 5000 )
			{
				/* move to the "CAR_DECELERATION" state after 5 sec & make counter = 0 */
				car_state = CAR_DECELERATION;
				state_change_millis_counter = 0;
				
				/* prepare speed to be 100 for the "CAR_DECELERATION" state */
				dutyCycle = 100;
			}
		break;
		
		case CAR_DECELERATION:
			if ( speed_change_millis_counter >= 50 )
			{
				/* decrease duty cycle by 1% every 50 milli seconds & make counter = 0 */
				dutyCycle -= 1;
				speed_change_millis_counter = 0;
			}
			MotorDC_Dir(MOT_1, FORWARD);
			MotorDC_Dir(MOT_2, FORWARD);
		
			if ( state_change_millis_counter >= 5000 )
			{
				/* move to the "CAR_TURN_RIGHT" state after 5 sec & make counter = 0 */
				car_state = CAR_TURN_RIGHT;
				state_change_millis_counter = 0;
				
				/* prepare speed to be 50 for the "CAR_TURN_RIGHT" state */
				dutyCycle = 50;
			}
			break;
			
		case CAR_TURN_RIGHT:
			MotorDC_Dir(MOT_1, BACKWARD);
			MotorDC_Dir(MOT_2, FORWARD);
			if ( state_change_millis_counter >= 1000 )
			{
				/* move to the "CAR_ENGINE_OFF" state after 1 sec */
				car_state = CAR_ENGINE_OFF;
			}
			break;
		
		case CAR_ENGINE_OFF:
			/* stop the car forever */
			MotorDC_Dir(MOT_1, STOP);
			MotorDC_Dir(MOT_2, STOP);
			break;
	}
}

int main(void)
{	
	sei();
	timer1Init(T1_NORMAL_MODE, T1_OC1_DIS, T1_PRESCALER_NO,0,0,0,0,T1_INTERRUPT_CMP_1A | T1_INTERRUPT_NORMAL);
	MotorDC_Init(MOT_1);
	MotorDC_Init(MOT_2);
	
	/* duty sycle is initialized with 0 as global variable */
	ocr_ticks = ((dutyCycle * PWM_TCNT_TICKS) / 100) - 1;
	ocr_start = PWM_TCNT_START + ocr_ticks;
	OCR1A = ocr_start;
	timer1Set(PWM_TCNT_START);
	timer1Start();
	
    while (1) 
    {	
		cli();
		car_control();
		sei();	
    }
}

ISR(TIMER1_COMPA_VECTOR)
{
	gpioPinWrite(GPIOD, BIT5, 0);
	gpioPinWrite(GPIOD, BIT4, 0);
}

ISR(TIMER1_OVF_VECTOR)
{
	gpioPinWrite(GPIOD, BIT5, 1);
	gpioPinWrite(GPIOD, BIT4, 1);
	timer1Set(PWM_TCNT_START);
	
	/* these 2 counters are incremented every 1 milli second as the PWM period is 1 millis second [frequency = 1KHz] */
	state_change_millis_counter++;
	speed_change_millis_counter++;
	
	/* calculate the value to put in the OCR1A at every over flow interrupt based on the duty cycle change in the while(1) loop */
	ocr_ticks = ((dutyCycle * PWM_TCNT_TICKS) / 100) - 1;
	ocr_start = PWM_TCNT_START + ocr_ticks;
	OCR1A = ocr_start;
}
