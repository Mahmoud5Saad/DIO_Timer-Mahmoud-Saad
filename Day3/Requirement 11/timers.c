/*
 * timers.c
 *
 * Created: 12/7/2019 4:48:54 PM
 *  Author: Mahmoud Saad
 */ 

#include "timers.h"

#define MOTOR_OUT_1A_GPIO	(GPIOD)
#define MOTOR_OUT_1A_BIT	(BIT1)


En_timer0perscaler_t	g_preTimer0;
En_timer1perscaler_t	g_preTimer1;
En_timer2perscaler_t	g_preTimer2;
/*===========================Timer0 Control===============================*/
/**
 * Description:
 * @param control
 * @param initialValue
 * @param outputCompare
 * @param interruptMask
 */
void timer0Init(En_timer0Mode_t mode, En_timer0OC_t OC0, En_timer0perscaler_t prescal, uint8 initialValue, uint8 outputCompare, En_timer0Interrupt_t interruptMask)
{
	g_preTimer0 = prescal;
	TCCR0 = 0;
	OCR0 = outputCompare;
	TCCR0 |= OC0;
	TCCR0 |= mode;
	TIMSK |= interruptMask;
	TCNT0 = initialValue;
	TCCR0 = (prescal&0x07);
}

/**
 * Description:
 * @param value
 */
void timer0Set(uint8 value)
{
	TCNT0 = value;
}

/**
 * Description:
 * @return
 */
uint8 timer0Read(void)
{
	return TCNT0;
}

/**
 * Description:
 */
void timer0Start(void)
{
	TCCR0 |= g_preTimer0;
}

/**
 * Description:
 */
void timer0Stop(void)
{
	TCCR0 &= ~(0x07);
}

/**
 * Description:
 * @param delay
 */
void timer0Delay_ms(uint16 delay)
{
	uint16 i;
	timer0Set(225);
	for (i=0; i<delay; i++)
	{
		while(!(TIFR&(1)));
		TIFR|=1;
		timer0Set(225);
	}
}

/**
 * Description:
 * @param dutyCycle
 */
void timer0SwPWM(uint8 dutyCycle,uint8 freq);









/*===========================Timer1 Control===============================*/
/**
 * Description:
 * @param controlA
 * @param controlB
 * @param initialValue
 * @param outputCompare
 * @param interruptMask
 */
void timer1Init(En_timer1Mode_t mode,En_timer1OC_t OC,En_timer1perscaler_t prescal,\
 uint16 initialValue, uint16 outputCompareA,uint16 outputCompareB, uint16 inputCapture, En_timer1Interrupt_t interruptMask)
{
	/* stop the timer */
	TCCR1B &= ~(0x07);
	g_preTimer1 = prescal;
	OCR1A = outputCompareA;
	OCR1B = outputCompareB;
	TIMSK |= interruptMask;
	TCNT1 = initialValue;
	if ( inputCapture )
	{
		ICR1 = inputCapture;
	}
	TCCR1 |= (mode) | (OC);
	/* start the timer */
	TCCR1B |= (prescal&0x07) ;
 }

/**
 * Description:
 * @param value
 */
void timer1Set(uint16 value)
{
	TCNT1 = value;
}

/**
 * Description:
 * @return
 */
uint16 timer1Read(void)
{
	return TCNT1;
}

/**
 * Description:
 */
void timer1Start(void)
{
	TCCR1B |= (g_preTimer1&0x07);
}

/**
 * Description:
 */
void timer1Stop(void)
{
	TCCR1B &= ~(0x07);
}

/**
 * Description:
 * @param delay
 */
void timer1Delay_ms(uint16 delay)
{
	uint16 i;
	timer1Set(0xFFFF-31);
	for (i=0; i<delay; i++)
	{
		while(!(TIFR&(1<<2)));
		TIFR|=(1<<2);
		timer1Set(0xFFFF-31);
	}	
}

/**
 * Description:
 * @param dutyCycle
 */
void timer1SwPWM(uint8 dutyCycle,uint8 freq)
{
	gpioPinWrite(MOTOR_OUT_1A_GPIO, MOTOR_OUT_1A_BIT, HIGH);
	timer1Delay_ms(1);
	gpioPinWrite(MOTOR_OUT_1A_GPIO, MOTOR_OUT_1A_BIT, LOW);
	timer1Delay_ms(1);
}





/*===========================Timer2 Control===============================*/
/**
 * Description:
 * @param control
 * @param initialValue
 * @param outputCompare
 * @param interruptMask
 */
void timer2Init(En_timer2Mode_t mode,En_timer2OC_t OC,En_timer2perscaler_t prescal, uint8 initialValue, uint8 outputCompare, uint8 assynchronous, En_timer2Interrupt_t interruptMask)
{
	g_preTimer2 = prescal;
	TCCR2 = (prescal&0x07);
	if ( outputCompare )
	{
		TCCR2 |= (1<<7);
	}
	if ( !OC )
	{
		TCCR2 |= (OC&0x30);
	}
	TCCR2 |= (mode&0x08);
	TIMSK |= interruptMask;
	if ( !initialValue )
	{
		TCNT2 = initialValue;
	}
	if ( !outputCompare )
	{
		OCR2 = outputCompare;
	}
}
/**
 * Description:
 * @param value
 */
void timer2Set(uint8 value)
{
	TCNT2 |= value;
}

/**
 * Description:
 * @return
 */
uint8 timer2Read(void)
{
	return TCNT2;
}

/**
 * Description:
 */
void timer2Start(void)
{
	TCCR2 = (g_preTimer2&0x07);
}

/**
 * Description:
 */
void timer2Stop(void)
{
	TCCR2 &= ~(0x07);
}
#if(0)
/**
 * Description:
 * @param delay
 */
void timer2Delay_ms(uint16 delay)
{
	
}

/**
 * Description:
 * @param dutyCycle
 */
void timer2SwPWM(uint8 dutyCycle,uint8 freq)
{
	
}

/*
 * user defined
 */
void timer2Delay_us(uint32 delay)
{
	
}
#endif