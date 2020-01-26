/******************************************************************************
 *
 * 	Module: 		adc
 *
 *	File Name: 		adc.c
 *
 * 	Description: 	source file for adc
 *
 * 	Authors: 		Mahmoud Saad
 * 	
 *	Created on:		Jan 25, 2020
 *
 *******************************************************************************/

/************************************************************************/
/*				 INCLUDES			        */
/************************************************************************/
#include "adc.h"
#include "registers.h"

/************************************************************************/
/*				 DEFINES			        */
/************************************************************************/
/************************************************************************
 *			Typedefs Enumerations and structures declarations                                  
 ************************************************************************/
typedef enum ge_ADC_moduleStatusType
{
	ADC_DEINIT, ADC_INIT
}ge_ADC_moduleStatusType;
/*******************************************************************************
 *			Global variables
 *******************************************************************************/
ge_ADC_moduleStatusType ge_ADC_moduleStatus = ADC_DEINIT;
/************************************************************************/
/*		         ADC FUNCTIONS IMPLEMENTATION							*/
/************************************************************************/


/**
 * Input: Pointer to a structure contains the information needed to initialize the ADC.
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: Initiates the module, it sets the pre-scaler, Right/Left adjustment, auto trigger, channels to work on.
 *
 */
ERR_STATUS ADC_Init(str_ADC_cfg_t* str_ADC_cfg)
{
	ERR_STATUS retval;
	if (NULL==str_ADC_cfg)
	{
		retval = ADC_BASE + ERR_NULL_PTR;
	} 
	else
	{
		switch (str_ADC_cfg->u8_Interrupt_Polling)
		{
			case ADC_POLLING:
				ADCSRA &= ~(1<<3); /* disable interrupts */
				break;
			case ADC_INTERRUPT:
				ADCSRA |= (1<<3); /* enable interrupts */
				break;
		}
		ADCSRA |= (str_ADC_cfg->u8_PreScaler); /* put prescalar in registers */
		ADCSRA |= (1<<7); /* enable adc */
		ge_ADC_moduleStatus = ADC_INIT;
		retval = ERR_SUCCESS;
	}
	return retval;
}

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function enables the ADC conversion one time whenever called
 **/
ERR_STATUS ADC_Enable(void);
/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function starts the ADC conversion one time whenever called
 **/
ERR_STATUS ADC_Start_Polling(uint8 u8_ADC_Ch, uint16 *ADC_Read)
{
	ERR_STATUS retval;
	if (u8_ADC_Ch>7)
	{
		retval = ADC_BASE + ERR_INVALID_ARGU;
	}
	else if (ADC_DEINIT==ge_ADC_moduleStatus)
	{
		retval = ADC_BASE + ERR_NOT_INITIALIZED;
	}
	else
	{
		ADMUX = (ADMUX&0xF8) | (u8_ADC_Ch&0x07); /* choose channel */
		ADCSRA |= (1<<6); /* start conversion */
		while (BIT_IS_CLEAR(ADCSRA,4));
		/* polling until the ADC finishes conversion and sets the flag to 1 */
		SET_BIT(ADCSRA,4);
		/* clear the flag by writing 1 to it */
		*ADC_Read = ADC;
		retval = ERR_SUCCESS;
	}
	return retval;
}
/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function starts the ADC conversion one time whenever called
 **/
ERR_STATUS ADC_Start_Interrupt(uint8 u8_ADC_Ch);

/**
 * Input: Read variable pointer
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function Reads the ADC register
 **/
ERR_STATUS ADC_Read(uint16* ADC_Read)
{
	ERR_STATUS retval;
	if (NULL==ADC_Read)
	{
		retval = ADC_BASE + ERR_NULL_PTR;
	} 
	else if (ADC_DEINIT==ge_ADC_moduleStatus)
	{
		retval = ADC_BASE + ERR_NOT_INITIALIZED;
	}
	else
	{
		*ADC_Read = ADC;
		retval = ERR_SUCCESS;
	}
	return retval;
}


/**
 * Input: Auto Trigger Source for the ADC
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function sets the Auto Trigger Source for the ADC
 **/

ERR_STATUS ADC_Set_AutoTrig(uint8 u8_ADC_Ch, uint8 u8_AutoTrigSource);

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function stops the auto triggered ADC conversion
 *
 */

ERR_STATUS ADC_Stop_AutoTrig(void);

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function stops the call of the given function.
 *
 */
ERR_STATUS ADC_Stop(void);

/**
 * Input:
 * Output:
 * In/Out:
 * Return: The error status of the function.
 * Description: This function Disables the ADC conversion one time whenever called
 **/
ERR_STATUS ADC_Disable(void);
