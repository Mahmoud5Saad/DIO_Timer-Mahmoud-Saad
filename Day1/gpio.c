/******************************************************************************
 *
 *	File Name: gpio.c
 *
 * 	Description: source file for GPIO
 *
 * 	Authors: Mahmoud Saad
 *
 *	Created on: Dec 3, 2019
 *
 *******************************************************************************/


/*- INCLUDES ----------------------------------------------*/
#include "gpio.h"
/*- LOCAL MACROS ------------------------------------------*/
/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/
/*- GLOBAL STATIC VARIABLES -------------------------------*/
/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/
/*- APIs IMPLEMENTATION -----------------------------------*/

/*===========================PORT Control===============================*/
void gpioPortDirection(uint8 port, uint8 direction)
{
	switch(port)
	{
		case GPIOA:
			PORTA_DIR = direction;
			break;
		case GPIOB:
			PORTB_DIR = direction;
			break;
		case GPIOC:
			PORTC_DIR = direction;
			break;
		case GPIOD:
			PORTD_DIR = direction;
			break;
		default:
			break;
	}
}

void gpioPortWrite(uint8 port, uint8 value)
{
	switch(port)
	{
		case GPIOA:
			PORTA_DATA = value;
			break;
		case GPIOB:
			PORTB_DATA = value;
			break;
		case GPIOC:
			PORTC_DATA = value;
			break;
		case GPIOD:
			PORTD_DATA = value;
			break;
		default:
			break;
	}
}

void gpioPortToggle(uint8 port)
{
	switch(port)
	{
		case GPIOA:
			PORTA_DATA ^= 0xFF;
			break;
		case GPIOB:
			PORTB_DATA ^= 0xFF;
			break;
		case GPIOC:
			PORTC_DATA ^= 0xFF;
			break;
		case GPIOD:
			PORTD_DATA ^= 0xFF;
			break;
		default:
			break;
	}
}

uint8 gpioPortRead(uint8 port)
{
	uint8 result = 0;
	switch(port)
	{
		case GPIOA:
			result = PORTA_PIN;
			break;
		case GPIOB:
			result = PORTB_PIN;
			break;
		case GPIOC:
			result = PORTC_PIN;
			break;
		case GPIOD:
			result = PORTD_PIN;
			break;
		default:
			break;
	}
	return result;
}

/*===========================PIN Control===============================*/
void gpioPinDirection(uint8 port, uint8 pins, uint8 direction)
{
	switch(port)
	{
		case GPIOA:
			if (direction == HIGH)
			{
				PORTA_DIR |= pins;
			}
			else if (direction == LOW)
			{
				PORTA_DIR &= (~pins);
			}
			break;
		case GPIOB:
			if (direction == HIGH)
			{
				PORTB_DIR |= pins;
			}
			else if (direction == LOW)
			{
				PORTB_DIR &= (~pins);
			}
			break;
		case GPIOC:
			if (direction == HIGH)
			{
				PORTC_DIR |= pins;
			}
			else if (direction == LOW)
			{
				PORTC_DIR &= (~pins);
			}
			break;
		case GPIOD:
			if (direction == HIGH)
			{
				PORTD_DIR |= pins;
			}
			else if (direction == LOW)
			{
				PORTD_DIR &= (~pins);
			}
			break;
		default:
			break;
	}
}


void gpioPinWrite(uint8 port, uint8 pins, uint8 value)
{
	switch(port)
	{
		case GPIOA:
			if (value == HIGH)
			{
				PORTA_DATA |= pins;
			}
			else if (value == LOW)
			{
				PORTA_DATA &= (~pins);
			}
			break;
		case GPIOB:
			if (value == HIGH)
			{
				PORTB_DATA |= pins;
			}
			else if (value == LOW)
			{
				PORTB_DATA &= (~pins);
			}
			break;
		case GPIOC:
			if (value == HIGH)
			{
				PORTC_DATA |= pins;
			}
			else if (value == LOW)
			{
				PORTC_DATA &= (~pins);
			}
			break;
		case GPIOD:
			if (value == HIGH)
			{
				PORTD_DATA |= pins;
			}
			else if (value == LOW)
			{
				PORTD_DATA &= (~pins);
			}
			break;
		default:
			break;
	}
}

void gpioPinToggle(uint8 port, uint8 pins)
{
	switch(port)
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
}

uint8 gpioPinRead(uint8 port, uint8 pin)
{
	uint8 value = 0;
	uint8 shift = 0;
	switch(port)
	{
		case GPIOA:
			value = (PORTA_DATA&pin);
			break;
		case GPIOB:
			value = (PORTB_DATA&pin);
			break;
		case GPIOC:
			value = (PORTC_DATA&pin);
			break;
		case GPIOD:
			value = (PORTD_DATA&pin);
			break;
		default:
			break;
	}
	while (pin!=0)
	{
		shift++;
		pin = pin>>1;
	}
	return (value>>(shift-1));
}


/*===========================Upper Nibble Control===============================*/
/**
 * Description: set the upper nibble direction
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @param direction
 */
void gpioUpperNibbleDirection(uint8 port, uint8 direction)
{
	direction &= 0xF0;
	gpioPortDirection(port, direction );
}

/**
 * Description: write the upper nibble value
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @param value
 */
void gpioUpperNibbleWrite(uint8 port, uint8 value)
{
	value &= 0xF0;
	gpioPortWrite(port, value);
}

/**
 * Description: toggle the upper nibble value
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 */
void gpioUpperNibbleToggle(uint8 port)
{
	switch(port)
	{
		case GPIOA:
			PORTA_DATA ^= 0xF0;
			break;
		case GPIOB:
			PORTB_DATA ^= 0xF0;
			break;
		case GPIOC:
			PORTC_DATA ^= 0xF0;
			break;
		case GPIOD:
			PORTD_DATA ^= 0xF0;
			break;
		default:
			break;
	}
}

/**
 * Description: read the upper nibble value
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @return
 */
uint8 gpioUpperNibbleRead(uint8 port)
{
	uint8 result = gpioPortRead(port);
	result &= 0xF0;
	return result;
}

/*===========================Lower Nibble Control===============================*/
/**
 * Description: set the lower nibble direction
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 *
 * @param direction
 */
void gpioLowerNibbleDirection(uint8 port, uint8 direction)
{
	direction &= 0x0F;
	gpioPortDirection(port, direction );
}

/**
 * Description: write the lower nibble value
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @param value
 */
void gpioLowerNibbleWrite(uint8 port, uint8 value)
{
	value &= 0x0F;
	gpioPortWrite(port, value);
}

/**
 * Description: toggle the lower nibble value
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 *
 */
void gpioLowerNibbleToggle(uint8 port)
{
	switch(port)
	{
		case GPIOA:
			PORTA_DATA ^= 0x0F;
			break;
		case GPIOB:
			PORTB_DATA ^= 0x0F;
			break;
		case GPIOC:
			PORTC_DATA ^= 0x0F;
			break;
		case GPIOD:
			PORTD_DATA ^= 0x0F;
			break;
		default:
			break;
	}
}

/**
 * Description: read the lower nibble value
 * @param port: portID and takes the following values
 * 				- 0 -> GPIOA
 * 				- 1 -> GPIOB
 * 				- 2 -> GPIOC
 * 				- 3 -> GPIOD
 * @return
 */
uint8 gpioLowerNibbleRead(uint8 port)
{
	uint8 result = gpioPortRead(port);
	result &= 0x0F;
	return result;
}
