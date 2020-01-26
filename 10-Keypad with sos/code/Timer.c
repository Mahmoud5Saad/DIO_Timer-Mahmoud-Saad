/******************************************************************************
 *
 * 	Module:			Timer
 *
 *	File Name:		Timer.c
 *
 * 	Description: 
 *
 * 	Authors:		Mahmoud Saad
 * 	
 *	Created on:		Sunday, 22-Dec-19 2:13:51 PM
 *
 *******************************************************************************/

/*- INCLUDES ----------------------------------------------*/
#include	"Timer.h"
#include	"registers.h"
#include	"interrupt.h"
/*- LOCAL MACROS ------------------------------------------*/
/* structure check values */
#define		TIMER_MAX_CHANNEL_NO			2
#define		TIMER_MAX_MODE					2
#define		TIMER_MAX_INTEERUPT				1
#define		TIMER_MAX_PRESCALAR				7

/* timer states */
#define		TIMER_UNINT			0
#define		TIMER_INIT			1
#define		TIMER_RUNNING		2

/* counter mode prescalars */
#define		TIMER_0_1_COUNT_RISING_EDGE			6
#define		TIMER_0_1_COUNT_FALLING_EDGE		7

/* check maximum counts */
#define		TIMER_0_2_MAX_COUNT					(0xFF)
#define		TIMER_1_MAX_COUNT					(0xFFFF)

/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/
/*- GLOBAL STATIC VARIABLES -------------------------------*/
/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/
/*- APIs IMPLEMENTATION -----------------------------------*/

/*******************************************************************************
 *			Preprocessor Macros                                    
 *******************************************************************************/
/*******************************************************************************
 *			Enumerations and structures declarations                                  
 *******************************************************************************/
/*******************************************************************************
 *			Private functions Prototypes                                   
 *******************************************************************************/
/*******************************************************************************
 *			Global variables
 *******************************************************************************/
/*******************************************************************************
 *			Static global variables
 *******************************************************************************/
/* prescalars of timers */
const static uint8 gau_timersRegisterPrescalar[3][8] = 
{
	{ 0, 1, 2, 0, 3, 0, 4, 5 },
	{ 0, 1, 2, 0, 3, 0, 4, 5 },
	{ 0, 1, 2, 3, 4, 5, 6, 7 }
};	
/* store indices of prescalars */
static uint16  gau_timersPrescalars[3] = {0};

/* array for timer modes */
static uint8 gau_timerState[3] = { TIMER_UNINT, TIMER_UNINT, TIMER_UNINT };

/* array of pointers to functions for call back */
void (*gpf_PtrCbk[3])(void);

/*******************************************************************************
 *			Extern Global variables
 *******************************************************************************/
 /*******************************************************************************
*			Functions Definitions                                  
 *******************************************************************************/
 /**
 * Input: Pointer to a structure contains the information needed to initialize the timer. 
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: Initiates the module.
 * 							
 */
ERROR_STATUS Timer_Init(Timer_cfg_s* Timer_cfg)
{
	/* local variable to indicate unique prescalar */
	uint8 Prescalar_32_or_128 = FALSE;
	/* check the unique state of prescalars */
	if ( (TIMER_PRESCALER_32 == Timer_cfg->Timer_Prescaler) || (TIMER_PRESCALER_128 == Timer_cfg->Timer_Prescaler) )
	{
		Prescalar_32_or_128 = TRUE;
	}
	/* check that the timer is not initialized before */
	if ( !( TIMER_UNINT == gau_timersPrescalars[Timer_cfg->Timer_CH_NO] ) || (NULL == Timer_cfg->Timer_Cbk_ptr) )
	{
		return E_NOK;
	}
	/* check that pointer is not null */
	else if ( NULL == Timer_cfg )
	{
		return E_NOK;
	}
	/* check on input of structures */
	else if	(	((Timer_cfg->Timer_CH_NO) > TIMER_MAX_CHANNEL_NO) || \
				((Timer_cfg->Timer_Mode) > TIMER_MAX_MODE) || \
				((Timer_cfg->Timer_Polling_Or_Interrupt) > TIMER_MAX_INTEERUPT) || \
				((Timer_cfg->Timer_Prescaler) > TIMER_MAX_PRESCALAR) )
	{
		return E_NOK;
	}
	/* check the prescalars that doesn't exist in timer 0 and 1 */
	else if (	((TIMER_CH0 == Timer_cfg->Timer_CH_NO)&& TRUE == Prescalar_32_or_128 ) || \
				((TIMER_CH1 == Timer_cfg->Timer_CH_NO)&& TRUE == Prescalar_32_or_128 ) )
	{
		return E_NOK;
	}
	else
	{
		/* save pointer to callback function */
		gpf_PtrCbk[Timer_cfg->Timer_CH_NO] = Timer_cfg->Timer_Cbk_ptr;
		/* save prescalar in global array for each timer */
		switch (Timer_cfg->Timer_Mode)
		{
			case TIMER_MODE:
				/* store prescalar in global array with index in timer mode */
				gau_timersPrescalars[Timer_cfg->Timer_CH_NO] = \
								gau_timersRegisterPrescalar[Timer_cfg->Timer_CH_NO][Timer_cfg->Timer_Prescaler];
				break;
				/*  store prescalar in global array with index even in timer 2 case */
				case COUNTER_RISING_EDGE_MODE:
					gau_timersPrescalars[Timer_cfg->Timer_CH_NO] = TIMER_0_1_COUNT_RISING_EDGE;
					break;
				case COUNTER_FALLING_EDGE_MODE:
					gau_timersPrescalars[Timer_cfg->Timer_CH_NO] = TIMER_0_1_COUNT_FALLING_EDGE;
					break;				
		}
		/* if timer is interrupt set bits in register */
		if ( TIMER_INTERRUPT_MODE == Timer_cfg->Timer_Polling_Or_Interrupt )
		{
			switch (Timer_cfg->Timer_CH_NO)
			{
				case TIMER_CH0:
					TCCR0 |= (1<<3); /* enable timer in compare mode */
					TIMSK |= (1<<1); /* Timer/Counter0 Output Compare Match Interrupt Enable */
					break;
				case TIMER_CH1:
					TCCR1B |= (1<<3); /* enable timer in compare mode */
					TIMSK |= (1<<4); /* Timer/Counter1, Output Compare A Match Interrupt Enable */
					break;
				case TIMER_CH2:
					TCCR2 |= (1<<3); /* enable timer in compare mode */
					TIMSK |= (1<<7); /* Timer/Counter2 Output Compare Match Interrupt Enable */
					break;				
			}
		}
		else {}
		gau_timerState[Timer_cfg->Timer_CH_NO] = TIMER_INIT;
		return E_OK;
	}
}

/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to be started.
 *	Timer_Count: The start value of the timer.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function starts the needed timer.
 * 							
 */
ERROR_STATUS Timer_Start(uint8 Timer_CH_NO, uint16 Timer_Count)
{
	if ( !(TIMER_INIT == gau_timerState[Timer_CH_NO]) )
	{
		return E_NOK;
	}
	else if ( (Timer_CH_NO > TIMER_MAX_CHANNEL_NO) )
	{
		return E_NOK;
	}
	else if (	( (Timer_Count > TIMER_0_2_MAX_COUNT ) && ( (TIMER_CH0 == Timer_CH_NO) || \
				(TIMER_CH2 == Timer_CH_NO) )  ) )
	{
		return E_NOK;
	}
	else
	{
		switch (Timer_CH_NO)
		{
			case TIMER_CH0:
			TCNT0 = 0; /*initialize timer to zero */
			OCR0 = Timer_Count;
			TCCR0 = ( TCCR0 & ~(0x07) ) | ((uint8)(gau_timersPrescalars[Timer_CH_NO]));
			break;
			case TIMER_CH1:
			TCNT1 = 0; /*initialize timer to zero */
			OCR1A = Timer_Count;
			TCCR1B = ( TCCR1B & ~(0x07) ) | gau_timersPrescalars[Timer_CH_NO];
			break;
			case TIMER_CH2:
			TCNT2 = 0; /*initialize timer to zero */
			OCR2 = Timer_Count;
			TCCR2 = ( TCCR2 & ~(0x07) ) | ((uint8)(gau_timersPrescalars[Timer_CH_NO]));
			break;
		}
		gau_timerState[Timer_CH_NO] = TIMER_RUNNING;
		return E_OK;
	}
}

/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to be stopped.
 * Output:
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function stops the needed timer.
 * 							
 */
ERROR_STATUS Timer_Stop(uint8 Timer_CH_NO)
{
	if ( !(TIMER_RUNNING == gau_timerState[Timer_CH_NO]) )
	{
		return E_NOK;
	}
	else if ( (Timer_CH_NO > TIMER_MAX_CHANNEL_NO) )
	{
		return E_NOK;
	}
	else
	{
		switch (Timer_CH_NO)
		{
			case TIMER_CH0:
				TCCR0 &= ~(0x07);
				break;
			case TIMER_CH1:
				TCCR1B &= ~(0x07);
				break;
			case TIMER_CH2:
				TCCR2 &= ~(0x07);
				break;
			default:
				break;
		}
		gau_timerState[Timer_CH_NO] = TIMER_RUNNING;
		return E_OK;
	}
}

/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to get its status.
 * Output:
 * 	Data: A variable of type bool returns if the flag of the timer is raised or not.
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function is used to return if the flag of the timer is raised or not.
 * 							
 */
ERROR_STATUS Timer_GetStatus(uint8 Timer_CH_NO, bool* Data)
{
	/* check input variables and pointer and timer state */
	if ( (Timer_CH_NO > TIMER_MAX_CHANNEL_NO) || (NULL == Data) || ( TIMER_UNINT != gau_timerState[Timer_CH_NO]) )
	{
		return E_NOK;
	}
	else
	{
		switch(Timer_CH_NO)
		{
			case TIMER_CH0:
				*Data = ((TIFR) & (1<<1)/(1<<1)); /* reads OCF0: Output Compare Flag 0 */
				break;
			case TIMER_CH1:
				*Data = ((TIFR) & (1<<4)/(1<<4)); /* reads OCF1A: Timer/Counter1, Output Compare A Match Flag */
				break;				
			case TIMER_CH2:
				*Data = ((TIFR) & (1<<7)/(1<<7)); /* reads OCF2: Output Compare Flag 2 */
				break;
		}
		return E_OK;
	}
}

/**
 * Input: 
 * 	Timer_CH_NO: The channel number of the timer needed to get its value.
 * Output:
 * 	Data: This is the output variable of the function which holds the value of the timer.
 * In/Out:			
 * Return: The error status of the function.			
 * Description: This function is used to return the value of the timer.
 * 							
 */
ERROR_STATUS Timer_GetValue(uint8 Timer_CH_NO, uint16* Data)
{
	/* check input variables and pointer and timer state */
	if ( (Timer_CH_NO > TIMER_MAX_CHANNEL_NO) || (NULL == Data) || ( TIMER_UNINT == gau_timerState[Timer_CH_NO]) )
	{
		return E_NOK;
	}
	else
	{
		switch (Timer_CH_NO)
		{
			case TIMER_CH0:
				*Data = TCNT0;
				break;
			case TIMER_CH1:
				*Data = TCNT1;
				break;
			case TIMER_CH2:
				*Data = TCNT2;
				break;
		}
		return E_OK;
	}
}

/* isr implementations */
ISR(TIMER0_COMP_vect)
{
	(*gpf_PtrCbk[TIMER_CH0])();
}

ISR(TIMER1_COMPA_vect)
{
	(*gpf_PtrCbk[TIMER_CH1])();
}

ISR(TIMER2_COMP_vect)
{
	(*gpf_PtrCbk[TIMER_CH2])();
}