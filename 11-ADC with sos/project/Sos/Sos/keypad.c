/******************************************************************************
 *
 * 	Module: Keypad
 *
 *	File Name: kaypad.c
 *
 * 	Description: source file for keypad driver
 *
 * 	Author: Mahmoud Saad
 *
 * 	Created on: Apr 25, 2019
 *
 *******************************************************************************/
/*******************************************************************************
 *			INCLUDES                                    
 *******************************************************************************/
#include "keypad.h"

/*******************************************************************************
 *			Preprocessor Macros                                    
 *******************************************************************************/
#define KEYPAD_NUMBER_OF_KEYS	(9U)
#define KEYPAD_ROWS_SHIFT		(2U)
/*******************************************************************************
 *			Enumerations and structures declarations                                  
 *******************************************************************************/
/*******************************************************************************
 *			Global variables
 *******************************************************************************/
#if (COLUMNS == 3)
Keypad_buttonState Keypad_buttonStatus[KEYPAD_NUMBER_OF_KEYS];
#elif (COLUMNS == 4)
#endif
/*******************************************************************************
 *			Functions Definitions
 *******************************************************************************/
ERR_STATUS Keypad_init(void)
{
	uint8 index;
	for (index=0; index<9; index++)
	{
		Keypad_buttonStatus[index] = KEYPAD_RELEASED;
	}
	return ERR_SUCCESS;
}
/**********************************************************************************************
 * 	Function name: Keypad_Update
 *
 * 	Brief: returns error status
 *
 * 	Details: enters loop check rows the key is pressed going through the rows row by row
 * 	to set the one row as output and the rest with high impedance then loop on the coulmns
 * 	if any is pressed store it in an array
 * 	
 * 	Arguments:
 * 		-none
 *
 * 	Return:
 * 		- ERR_STATUS
 **********************************************************************************************/
void Keypad_Update(void)
{
	Keypad_buttonState e_previous_buttonState;
	uint8 col, row;
	/* scan the keypad */
	for ( col = 0; col < COLUMNS; col++)
	{
		KEYPAD_PORT_DIR = (0x20<<col);
		KEYPAD_PORT_OUT = (~(0x20<<col));
		for ( row = KEYPAD_ROWS_SHIFT; row < ROWS+KEYPAD_ROWS_SHIFT; row++)
		{
			if (BIT_IS_CLEAR(KEYPAD_PORT_IN,row))
			{
				#if (COLUMNS == 3)
				e_previous_buttonState = Keypad_buttonStatus[((row-KEYPAD_ROWS_SHIFT)*COLUMNS)+col];
				/* check on decision */
				switch (e_previous_buttonState)
				{
					case KEYPAD_RELEASED:
						Keypad_buttonStatus[((row-KEYPAD_ROWS_SHIFT)*COLUMNS)+col] = KEYPAD_PREPRESSED;
						break;
					case KEYPAD_PREPRESSED:
						Keypad_buttonStatus[((row-KEYPAD_ROWS_SHIFT)*COLUMNS)+col] = KEYPAD_PRESSED;
						break;
					case KEYPAD_PRESSED:
						Keypad_buttonStatus[((row-KEYPAD_ROWS_SHIFT)*COLUMNS)+col] = KEYPAD_PRESSED;
						break;
					case KEYPAD_PRERELEASED:
						Keypad_buttonStatus[((row-KEYPAD_ROWS_SHIFT)*COLUMNS)+col] = KEYPAD_PRESSED;
						break;
					default:
						/* do nothing */
						break;
				}
				#elif (COLUMNS == 4)
				#endif
			}
			else
			{
				#if (COLUMNS == 3)
				e_previous_buttonState = Keypad_buttonStatus[((row-KEYPAD_ROWS_SHIFT)*COLUMNS)+col];
				switch (e_previous_buttonState)
				{
					case KEYPAD_RELEASED:
						Keypad_buttonStatus[((row-KEYPAD_ROWS_SHIFT)*COLUMNS)+col] = KEYPAD_RELEASED;
						break;
					case KEYPAD_PREPRESSED:
						Keypad_buttonStatus[((row-KEYPAD_ROWS_SHIFT)*COLUMNS)+col] = KEYPAD_RELEASED;
						break;
					case KEYPAD_PRESSED:
						Keypad_buttonStatus[((row-KEYPAD_ROWS_SHIFT)*COLUMNS)+col] = KEYPAD_PRERELEASED;
						break;
					case KEYPAD_PRERELEASED:
						Keypad_buttonStatus[((row-KEYPAD_ROWS_SHIFT)*COLUMNS)+col] = KEYPAD_RELEASED;
						break;
					default:
						/* do nothing */
						break;
				}
				#elif (COLUMNS == 4)
				#endif
			}
		}
	}
}
/**********************************************************************************************
 * 	Function name: KEYPAD_getPressedKey
 *
 * 	Brief: returns the pressed key on keypad
 *
 * 	Details: enters an infinite loop until the key is pressed going through the rows row by row
 * 	to set the one row as output and the rest with high impedence then loop on the coulmns
 * 	if any is pressed pass it to the adjust keynumber static function to modify it then
 * 	return the outcome to the caller
 *
 * 	Arguments:
 * 		-none
 *
 * 	Return:
 * 		-pressed key
 **********************************************************************************************/
uint8 KEYPAD_getPressedKey(uint8 u8_keyNumber)
{
	uint8 u8_pressedKey = Keypad_buttonStatus[u8_keyNumber];
	return u8_pressedKey;
}