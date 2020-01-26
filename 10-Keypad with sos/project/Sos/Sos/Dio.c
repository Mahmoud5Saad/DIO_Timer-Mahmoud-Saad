/******************************************************************************
 *
 * 	Module:			Dio
 *
 *	File Name:		Dio.c
 *
 * 	Description: 
 *
 * 	Authors:		Mahmoud Saad
 * 	
 *	Created on:		Sunday, 22-Dec-19 2:58:04 PM
 *
 *******************************************************************************/

/*******************************************************************************
 *			INCLUDES                                    
 *******************************************************************************/
#include "DIO.h"
#include "registers.h"
/*******************************************************************************
 *			Preprocessor Macros                                    
 *******************************************************************************/
#define		DIO_MAX_GPIO		3
#define		DIO_MAX_PINS		0xFF
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
/*******************************************************************************
 *			Extern Global variables
 *******************************************************************************/
 /*******************************************************************************
*			Functions Definitions                                  
 *******************************************************************************/
 /*
 *Input: DIO_Cfg_s -> to get PORT name, pins to be initiated and the required direction
 *Output: No output
 *In/Out:
 *Description: This function can set the direction of a full port, a nibble
 * 			  or even one pin.
 */
 ERROR_STATUS DIO_init (DIO_Cfg_s *DIO_info)
 {
	 /* check if pointer is null */
	 if ( NULL == DIO_info )
	 {
		 return E_NOK;
	 }
	 /* check members of structure */
	 else if (	((DIO_info->GPIO) > DIO_MAX_GPIO) ||\
				((DIO_info->pins) > DIO_MAX_PINS) ||\
				!( (DIO_info->dir) == OUTPUT)	|| ((DIO_info->dir) == INPUT) ) 
	 {
		 return E_NOK;
	 }
	 /* else implement the code */
	 else
	 {
		 switch (DIO_info->GPIO)
		 {
			 case GPIOA:
				PORTA_DIR = ( PORTA_DIR & ~(DIO_info->pins) ) | ( (DIO_info->pins) & (DIO_info->dir) );
				break;
			 case GPIOB:
				PORTB_DIR = ( PORTB_DIR & ~(DIO_info->pins) ) | ( (DIO_info->pins) & (DIO_info->dir) );
				break;
			 case GPIOC:
				PORTC_DIR = ( PORTC_DIR & ~(DIO_info->pins) ) | ( (DIO_info->pins) & (DIO_info->dir) );
				break;
			 case GPIOD:
				PORTD_DIR = ( PORTD_DIR & ~(DIO_info->pins) ) | ( (DIO_info->pins) & (DIO_info->dir) );
				break;
			 default:
			 break;				
		 }
		 return E_OK;
	 }	 
 }


 /*
 *Input: GPIO -> to get PORT name
 *					- GPIOA
 *					- GPIOB
 *					- GPIOC
 *					- GPIOD
 *		pins -> pins to be written at.
 *					- PIN0
 *					- PIN1
 *					- PIN2
 *					- PIN3
 *					- PIN4
 *					- PIN5
 *					- PIN6
 *					- PIN7
 *					- UPPER_NIBBLE
 *					- LOWER_NIBBLE
 *					- FULL_PORT
 *		value 	-> The desired value
 *Output: No output
 *In/Out: No In/Out
 *Description: This function can set the value of a full port, a nibble
 * 			  or even one pin.
 */
 ERROR_STATUS DIO_Write (uint8 GPIO, uint8 pins, uint8 value)
{
	/* check on input range */
	if (	(GPIO > DIO_MAX_GPIO) || (pins > DIO_MAX_PINS) || \
			!((value == HIGH) || (value == LOW) ) )
	{
		return E_NOK;
	}
	 /* else implement the code */
	 else
	 {
		 switch (GPIO)
		 {
		 case GPIOA:
			 PORTA_DATA = ( PORTA_DATA & ~(pins) ) | ( (pins) & (value) );
			 break;
		 case GPIOB:
			 PORTB_DATA = ( PORTB_DATA & ~(pins) ) | ( (pins) & (value) );
			 break;
		 case GPIOC:
			 PORTC_DATA = ( PORTC_DATA & ~(pins) ) | ( (pins) & (value) );
			 break;
		 case GPIOD:
			 PORTD_DATA = ( PORTD_DATA & ~(pins) ) | ( (pins) & (value) );
			 break;
		 default:
			 break;
		 }
		 return E_OK;
	 }
}

 /*
 *Input: GPIO -> to get PORT name
 *					- GPIOA
 *					- GPIOB
 *					- GPIOC
 *					- GPIOD
 *		pins -> pins to be written at.
 *					- PIN0
 *					- PIN1
 *					- PIN2
 *					- PIN3
 *					- PIN4
 *					- PIN5
 *					- PIN6
 *					- PIN7
 *					- UPPER_NIBBLE
 *					- LOWER_NIBBLE
 *					- FULL_PORT
 *Output: data -> the acquired data wether it was PORT data or pins data
 *In/Out:
 *Description: This function gets the value of a full port, a nibble
 * 			  or even one pin.
 */
 ERROR_STATUS DIO_Read (uint8 GPIO,uint8 pins, uint8 *data)
 {
	 /* check on input range */
	 if (	(GPIO > DIO_MAX_GPIO) || (pins > DIO_MAX_PINS) )
	 {
		 return E_NOK;
	 }
	 /* else implement the code */
	 else
	 {
		 switch (GPIO)
		 {
			 case GPIOA:
			 *data = ( PORTA_PIN & (pins) );
			 break;
			 case GPIOB:
			 *data = ( PORTB_PIN & (pins) );
			 break;
			 case GPIOC:
			 *data = ( PORTC_PIN & (pins) );
			 break;
			 case GPIOD:
			 *data = ( PORTD_PIN & (pins) )	;
			 break;
			 default:
			 break;
		 }
		 return E_OK;
	 }
 }

 /*
 *Input: GPIO -> to get PORT name
 *					- GPIOA
 *					- GPIOB
 *					- GPIOC
 *					- GPIOD
 *		pins -> pins to be written at.
 *					- PIN0
 *					- PIN1
 *					- PIN2
 *					- PIN3
 *					- PIN4
 *					- PIN5
 *					- PIN6
 *					- PIN7
 *					- UPPER_NIBBLE
 *					- LOWER_NIBBLE
 *					- FULL_PORT
 *Output: data -> No output
 *In/Out:
 *Description: This function toggles the value of a full port, a nibble
 * 			  or even one pin.
 */
 ERROR_STATUS DIO_Toggle (uint8 GPIO, uint8 pins)
 {
	/* check on input range */
	if (	(GPIO > DIO_MAX_GPIO) || (pins > DIO_MAX_PINS) )
	{
		return E_NOK;
	}
	/* else implement the code */
	else
	{
		switch (GPIO)
		{
			case GPIOA:
			PORTA_DATA ^= pins;
			break;
			case GPIOB:
			PORTB_DATA ^= pins;
			break;
			case GPIOC:
			PORTC_DATA ^= pins;
			break;
			case GPIOD:
			PORTD_DATA ^= pins;
			break;
			default:
			break;
		}
		return E_OK;
	}	 
 }