/*
 * timers.c
 *
 * Created: 12/7/2019 4:48:54 PM
 *  Author: Mahmoud Saad
 */ 

#include "timers.h"

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
void timer0Init(En_timer0Mode_t mode,En_timer0OC_t OC0,En_timer0perscaler_t prescal, uint8 initialValue, uint8 outputCompare, uint8 interruptMask)
{
	g_preTimer0 = prescal;
	TCCR0 = (prescal&0x07);
	if ( !outputCompare )
	{
		TCCR0 |= (1<<7);
	}
	if ( !OC0 )
	{
		TCCR0 |= (OC0&0x30);
	}
	TCCR0 |= (mode&0x08);
	TIMSK |= interruptMask;
	if ( !initialValue )
	{
		TCNT0 = initialValue;
	}
	if ( !outputCompare )
	{
		OCR0 = outputCompare;
	}
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
void timer0Delay_ms(uint16 delay);

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
 uint16 initialValue, uint8 outputCompareLow, uint8 outputCompareHigh,uint16 inputCapture, uint8 interruptMask)
{
	g_preTimer1 = prescal;
	TCCR1B = (prescal&0x07);
	TCCR1B |= (mode&0x08);
	if ( !outputCompareHigh )
	{
		TCCR1A |= (1<<3);
		OCR1A = inputCapture;
	}
	if ( !outputCompareLow )
	{
		TCCR1A |= (1<<2);
		OCR1B = inputCapture;
	}
	if ( !OC )
	{
		TCCR1 |= (OC&0xC000);
	}
	TIMSK |= interruptMask;
	TCNT0 = initialValue;
	if ( inputCapture )
	{
		ICR1 = inputCapture;
	}
 }

/**
 * Description:
 * @param value
 */
void timer1SetA(uint8 value)
{
	TCNT1 = value;
}

/**
 * Description:
 * @param value
 */
void timer1SetB(uint8 value)
{
	TCNT1 = value;
}

/**
 * Description:
 * @return
 */
uint8 timer1ReadA(void)
{
	return TCNT1;	
}

/**
 * Description:
 * @return
 */
uint8 timer1ReadB(void)
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
void timer1Delay_ms(uint16 delay);

/**
 * Description:
 * @param dutyCycle
 */
void timer1SwPWM(uint8 dutyCycle,uint8 freq);





/*===========================Timer2 Control===============================*/
/**
 * Description:
 * @param control
 * @param initialValue
 * @param outputCompare
 * @param interruptMask
 */
void timer2Init(En_timer2Mode_t mode,En_timer2OC_t OC,En_timer2perscaler_t prescal, uint8 initialValue, uint8 outputCompare, uint8 assynchronous, uint8 interruptMask)
{
	g_preTimer2 = prescal;
	TCCR2 = (prescal&0x07);
	if ( !outputCompare )
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

/**
 * Description:
 * @param delay
 */
void timer2Delay_ms(uint16 delay);

/**
 * Description:
 * @param dutyCycle
 */
void timer2SwPWM(uint8 dutyCycle,uint8 freq);

/*
 * user defined
 */
void timer2Delay_us(uint32 delay);