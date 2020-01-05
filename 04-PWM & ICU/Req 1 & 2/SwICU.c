/*
 * SwICU.c
 *
 * Created: 12/15/2019 12:19:24 PM
 *  Author: Amr
 */ 

#include "timers.h"
#include "SwICU.h"
#include "softwareDelay.h"
#include "interrupts.h"

volatile static uint8 ICU_Timer_Counts;
volatile uint8 trigger_flag = TRIGGER_SEND;
volatile uint32 ovf_counts = 0;

void SwICU_Init(SwICU_Edge_TypeDef inputCaptureEdge)
{
	
	timer0Init(T0_NORMAL_MODE, T0_OC0_DIS, T0_PRESCALER_1024, 0, 0, T0_INTERRUPT_NORMAL);
	gpioPinDirection(ECHO_GPIO, ECHO_PIn, INPUT);				// make INT2(ECHO pin) as input
	gpioPinDirection(TRIGGER_GPIO, TRIGGER_PIn, OUTPUT);		// make (TRIGGER pin) as output
	SwICU_Enable();												// enable INT2 interrupt
	SwICU_SetCfgEdge(inputCaptureEdge);							// set edge initially to rising
}

SwICU_Edge_TypeDef SwICU_GetCfgEdge(void)
{
	return (MCUCSR & 0x40);
}

void SwICU_SetCfgEdge(SwICU_Edge_TypeDef edge)
{
	MCUCSR &=~(0x40);			// clear the previous value before writing
	MCUCSR |= edge;				// write the new value
}

void SwICU_Read(volatile uint8 * capt)
{
	// configure to detect rising edge
	SwICU_SetCfgEdge(SwICU_EdgeRisiging);
	
	// send the trigger 10us pulse
	gpioPinWrite(TRIGGER_GPIO, TRIGGER_PIn, 1 );
	SwDelay_us(10);
	gpioPinWrite(TRIGGER_GPIO, TRIGGER_PIn, 0 );
	
	*capt = ICU_Timer_Counts;
}

void SwICU_Stop(void)
{
	timer0Stop();
}

void SwICU_Start(void)
{
	timer0Set(0);
	timer0Start();
}

void SwICU_Enable(void)
{
	GICR |= 1<<5;			// enable INT2 interrupt
}

void SwICU_Disable(void)
{
	GICR &=~(1<<5);			// disable INT2 interrupt
}


ISR(INT2_VECTOR)
{
	// if received interrupt for the rising edge:
	if ( gpioPinRead(ECHO_GPIO, ECHO_PIn) == 1 )
	{
		SwICU_SetCfgEdge(SwICU_EdgeFalling);			// change edge to falling
		SwICU_Start();									// then set to 0 & start the timer0 counting
	}
	
	// if received interrupt for the falling edge:
	else if ( gpioPinRead(ECHO_GPIO, ECHO_PIn) == 0 )
	{
		SwICU_SetCfgEdge(SwICU_EdgeRisiging);			// change edge to rising
		ICU_Timer_Counts = timer0Read();
		SwICU_Stop();
		trigger_flag = TRIGGER_SEND;
	}
}

ISR(TIMER0_OVF_VECTOR)
{
	ovf_counts++;
}