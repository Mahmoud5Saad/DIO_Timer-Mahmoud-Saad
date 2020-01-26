/******************************************************************************
 *
 * 	Module: Keypad
 *
 *	File Name: keypad.h
 *
 * 	Description: header file for keypad driver
 *
 * 	Author: Mahmoud Saad
 *
 *	Created on: Apr25, 2019
 *
 *******************************************************************************/


#ifndef INCLUDES_KEYPAD_H_
#define INCLUDES_KEYPAD_H_

/*******************************************************************************
 *			INCLUDES                                    
 *******************************************************************************/
#include "std_types.h"
#include "common_macros.h"
#include "common_retval.h"
#include "registers.h"

/*******************************************************************************
 *			Enumerations and structures declarations                                  
 *******************************************************************************/
typedef enum Keypad_buttonState
{
	KEYPAD_RELEASED, KEYPAD_PRESSED, KEYPAD_PRERELEASED, KEYPAD_PREPRESSED
}Keypad_buttonState;

/*******************************************************************************
 *			Preprocessor Macros
 *******************************************************************************/
/* Keypad configurations for number of rows and columns */
#define COLUMNS 3		/* sprints kit */
#define ROWS 3

/* keys */
#define KEYPAD_KEY_1 (0U)
#define KEYPAD_KEY_2 (1U)
#define KEYPAD_KEY_3 (2U)
#define KEYPAD_KEY_4 (3U)
#define KEYPAD_KEY_5 (4U)
#define KEYPAD_KEY_6 (5U)
#define KEYPAD_KEY_7 (6U)
#define KEYPAD_KEY_8 (7U)
#define KEYPAD_KEY_9 (8U)

/* Keypad Port Configurations */
#define KEYPAD_PORT_OUT PORTC_DATA
#define KEYPAD_PORT_IN  PORTC_PIN
#define KEYPAD_PORT_DIR PORTC_DIR

/*******************************************************************************
 *			Functions Prototypes
 *******************************************************************************/
ERR_STATUS Keypad_init(void);
/**********************************************************************************************
 * 	Function name: KEYPAD_getPressedKey
 *
 * 	Brief: returns the pressed key on keypad
 *
 * 	Details: enters an infinite loop until the key is pressed going through the rows row by row
 * 	to set the one row as output and the rest with high impedence then loop on the coulmns
 * 	if any is pressed pass it to the adjust key number static function to modify it then
 * 	return the outcome to the caller
 *
 * 	Arguments:
 * 		-none
 *
 * 	Return:
 * 		-pressed key
 **********************************************************************************************/
extern Keypad_buttonState KEYPAD_getPressedKey(uint8 u8_keyNumber);

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
extern void Keypad_Update(void);


#endif /* INCLUDES_KEYPAD_H_ */
