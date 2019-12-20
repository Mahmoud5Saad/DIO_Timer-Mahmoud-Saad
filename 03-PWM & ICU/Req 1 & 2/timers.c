/*
 * timers.c
 *
 * Created: 12/6/2019 3:01:27 PM
 *  Author: Amr
 */ 

#include "std_types.h"
#include "registers.h"
#include "timers.h"
#define F_CPU 16000000UL
static En_timer0perscaler_t timer0_prescaler_mask;
static En_timer1perscaler_t timer1_prescaler_mask;
static En_timer2perscaler_t timer2_prescaler_mask;

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
	timer0_prescaler_mask = prescal;
	TCCR0 = mode | OC0;
	TCNT0 = initialValue;
	OCR0 = outputCompare;
	TIMSK &=~(0b00000011);							// clear any previous values in the TOIE0, OCIE0 before writing on them
	TIMSK |= interruptMask;							// now, write these bits
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
	TCCR0 |= timer0_prescaler_mask;					// set prescaller clock
}

/**
 * Description:
 */
void timer0Stop(void)
{
	TCCR0 &=~(timer0_prescaler_mask);					// clear prescaller clock
}

/**
 * Description: using polling for now instead of interrupt
 * @param delay
 */
void timer0Delay_ms(uint16 delay)
{
	while (delay)
	{
		timer0Set( 256-((1.0/1000.0)/TIMER0_TICK_PERIOD) );
		timer0Start();
		while(!(TIFR & (1<<0)));
		TIFR |=1<<0;
		timer0Stop();
		delay--;
	}
}

/**
 * Description:
 * @param dutyCycle
 */
void timer0SwPWM(uint8 dutyCycle,uint8 freq)
{
	uint32 frequency = freq * 1000;
	uint32 tcnt_ticks = (F_CPU/(uint32)(frequency * 1));
	uint16 tcnt_start = 256 - tcnt_ticks;
	uint32 ocr_ticks = ((dutyCycle * tcnt_ticks) / 100) - 1;
	uint16 ocr_start = tcnt_start + ocr_ticks;
	
	gpioPinWrite(GPIOD, BIT5, 1);
	gpioPinWrite(GPIOD, BIT4, 1);
	OCR0 = ocr_start;
	timer0Set(tcnt_start);
	timer0Start();
	while(!(TIFR & (1<<1)));
	TIFR |=1<<1;
	gpioPinWrite(GPIOD, BIT5, 0);
	gpioPinWrite(GPIOD, BIT4, 0);
	while(!(TIFR & (1<<0)));
	TIFR |=1<<0;
	gpioPinWrite(GPIOD, BIT5, 1);
	gpioPinWrite(GPIOD, BIT4, 1);
	timer0Stop();
}




/*===========================Timer1 Control===============================*/
/**
 * Description:
 * @param controlA
 * @param controlB
 * @param initialValue
 * @param outputCompare
 * @param interruptMask
 */
void timer1Init(En_timer1Mode_t mode,En_timer1OC_t OC,En_timer1perscaler_t prescal, uint16 initialValue, uint16 outputCompareA, uint16 outputCompareB,uint16 inputCapture, En_timer1Interrupt_t interruptMask)
{
	timer1_prescaler_mask = prescal;
	TCCR1 = mode | OC;
	TCNT1 = initialValue;
	OCR1A = outputCompareA;
	OCR1A = outputCompareB;
	TIMSK &=~(0b00111100);							// clear any previous values in the TOIE1, OCIE1A, OCIE1B, TICIE1 before writing on them
	TIMSK |= interruptMask;							// now, write these bits
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
	TCCR1 |= timer1_prescaler_mask;					// set prescaller clock
}

/**
 * Description:
 */
void timer1Stop(void)
{
	TCCR1 &=~(timer1_prescaler_mask);					// clear prescaller clock
}

/**
 * Description:
 * @param delay
 */
void timer1Delay_ms(uint16 delay)
{
	while (delay)
	{
		timer1Set( 65536-((1.0/1000.0)/TIMER1_TICK_PERIOD) );
		timer1Start();
		while(!(TIFR & (1<<4)));
		TIFR |=1<<4;
		timer1Stop();
		delay--;
	}
}

/**
 * Description: function to make software pwm with timer1 using normal mode.
 * this function generates PWM on PORTD pins 4,5 (OC1A, OC1B)
 * you must use no prescaler when initializing the timer1
 * @param dutyCycle
 */
void timer1SwPWM(uint8 dutyCycle,uint8 freq)
{
	uint32 frequency = freq * 1000;
	uint32 tcnt_ticks = (F_CPU/(uint32)(frequency * TIMER1_PRESCALER));
	uint16 tcnt_start = 65536 - tcnt_ticks;
	uint32 ocr_ticks = ((dutyCycle * tcnt_ticks) / 100) - 1;
	uint16 ocr_start = tcnt_start + ocr_ticks;
	
	gpioPinWrite(GPIOD, BIT5, 1);
	gpioPinWrite(GPIOD, BIT4, 1);
	OCR1A = ocr_start;
	timer1Set(tcnt_start);
	timer1Start();
	while(!(TIFR & (1<<4)));
	TIFR |=1<<4;
	gpioPinWrite(GPIOD, BIT5, 0);
	gpioPinWrite(GPIOD, BIT4, 0);
	while(!(TIFR & (1<<2)));
	TIFR |=1<<2;
	gpioPinWrite(GPIOD, BIT5, 1);
	gpioPinWrite(GPIOD, BIT4, 1);
	timer1Stop();
}



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
	timer2_prescaler_mask = prescal;
	TCCR2 = mode | OC | assynchronous;
	TCNT2 = initialValue;
	OCR2 = outputCompare;
	TIMSK &=~(0b00000011);							// clear any previous values in the TOIE0, OCIE0 before writing on them
	TIMSK |= interruptMask;							// now, write these bits
}

/**
 * Description:
 * @param value
 */
void timer2Set(uint8 value)
{
	TCNT2 = value;
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
	TCCR2 |= timer2_prescaler_mask;					// set prescaller clock
}

/**
 * Description:
 */
void timer2Stop(void)
{
	TCCR2 &=~(timer2_prescaler_mask);					// clear prescaller clock
}

/**
 * Description:
 * @param delay
 */
void timer2Delay_ms(uint16 delay)
{
	while (delay)
	{
		timer0Set( 256-((1.0/1000.0)/TIMER0_TICK_PERIOD) );
		timer2Start();
		while(!(TIFR & (1<<0)));
		TIFR |=1<<0;
		timer0Stop();
		delay--;
	}
}

/**
 * Description:
 * @param dutyCycle
 */
void timer2SwPWM(uint8 dutyCycle,uint8 freq);

/*
 * user defined
 */
void timer2Delay_us(uint32 delay)
{
	
}