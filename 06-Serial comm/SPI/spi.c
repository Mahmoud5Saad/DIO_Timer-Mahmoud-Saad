/*
 * SPI_Pb.h
 *
 * Created: 12/28/2019 3:25:15 PM
 *  Author: Samra
 */
 /************************************************************************/
/*				 INCLUDES			        */
/************************************************************************/
#include "spi.h"
#include "DIO.h"
#include "char_lcd.h"
#include <stdlib.h>
 /************************************************************************/
/*			LOCAL MACROS/DEFINES FOR REGISTERS & PINS		        */
/************************************************************************/
#define SPI_DATA_REG    *((reg_type8)(0x2F))
#define SPI_STATUS_REG  *((reg_type8)(0x2E))
#define SPI_CTRL_REG    *((reg_type8)(0x2D))
/************************************************************************/
/*		         GLOBAL STATIC VARIABLES		        */
/************************************************************************/
static void (*gfp_SpiFunctionPointer) (void) = NULL;
volatile uint8 gu8_RecievedData = 0;
volatile uint8 gu8_RecievedFlag = FALSE;
/************************************************************************/
/*				 	SPI INTERRUPT SERVICE ROUTINES                  */
/************************************************************************/

/*
 * Function: ISR(SPI_STC_vect)
 * Inputs:	Takes function pointer value stored through initialization function in the static function pointer
 * Outputs:
 * In Out:
 * Return:
 * Description: Performs interrupt Service routine when SPI interrupt is triggered
 */


ISR(SPI_STC_vect)
{
    gu8_RecievedData = SPI_DATA_REG;
	gu8_RecievedFlag = TRUE;
	/*
	char str[10];
	itoa(gu8_RecievedData, str, 10);
	LCD_clear();
	LCD_send_string(str);
	*/	
    if (gfp_SpiFunctionPointer != NULL)
    {
        (*gfp_SpiFunctionPointer)();
    }
	
}


/************************************************************************/
/*		               APIS IMPLEMENTATION		        */
/************************************************************************/


/**
* @brief: Initialize SPI module
* @param:
* Input : SPI_Cfg_s* "Pointer to Struct of SPI Configration
* Output: None
* @return: Error codes if present
*/
ERROR_STATUS SPI_Init(SPI_Cfg_s* pstr_SPI_Confg)
{
    if (pstr_SPI_Confg == NULL)
    {
        return E_NOK;
    }
    SPI_CTRL_REG = 0x00;
    sei();
    DIO_Cfg_s Init_Pins;
    if (pstr_SPI_Confg->u8_SPIMode == SLAVE)
    {
        SPI_CTRL_REG = SPI_CTRL_REG | 0x00;
        Init_Pins.dir = INPUT;
        Init_Pins.GPIO = GPIOB;
        Init_Pins.pins = (PIN4 | PIN5 | PIN7);
        DIO_init(&Init_Pins);
        Init_Pins.dir = OUTPUT;
        Init_Pins.GPIO = GPIOB;
        Init_Pins.pins = (PIN1 | PIN3 | PIN6);
        DIO_init(&Init_Pins);
    }
    else if (pstr_SPI_Confg->u8_SPIMode == MASTER)
    {
        SPI_CTRL_REG = SPI_CTRL_REG | 0x10;
        Init_Pins.dir = OUTPUT;
        Init_Pins.GPIO = GPIOB;
        Init_Pins.pins = (PIN1 | PIN3 | PIN4 | PIN5 | PIN7);
        DIO_init(&Init_Pins);
        Init_Pins.dir = INPUT;
        Init_Pins.GPIO = GPIOB;
        Init_Pins.pins = (PIN6);
        DIO_init(&Init_Pins);
    }
    else
    {
        return E_NOK;
    }
    if (pstr_SPI_Confg->u8_DataOrder == LSB)
    {
        SPI_CTRL_REG = SPI_CTRL_REG | 0x20;
    }
    else if (pstr_SPI_Confg->u8_DataOrder == MSB)
    {
        SPI_CTRL_REG = SPI_CTRL_REG | 0x00;
    }
    else
    {
        return E_NOK;
    }
    if (pstr_SPI_Confg->u8_InterruptMode == INTERRUPT)
    {
        SPI_CTRL_REG = SPI_CTRL_REG | 0x80;
    }
    else if (pstr_SPI_Confg->u8_InterruptMode == POLLING)
    {
        SPI_CTRL_REG = SPI_CTRL_REG | 0x00;
    }
    else
    {
        return E_NOK;
    }
    gfp_SpiFunctionPointer = pstr_SPI_Confg->CBK_Func;
    SPI_CTRL_REG = SPI_CTRL_REG | 0x02;
    SPI_CTRL_REG = SPI_CTRL_REG | 0x40;
    return E_OK;
}

/**
* @brief: Transmit one byte over SPI
* @param:
* Input : u8_Data "Byte to be sent"
* Output: None
* @return: Error codes if present
*/
ERROR_STATUS SPI_SendByte(uint8 u8_Data)
{
    DIO_Write(GPIOB, PIN4, LOW);
    SPI_DATA_REG = u8_Data;
    while ((SPI_STATUS_REG & 0x80) == 0);
    DIO_Write(GPIOB, PIN4, HIGH);
    return E_OK;
}
/**
* @brief: Receive one byte over SPI
* @param:
* Input : None
* Output: *ptru8_Data "points to where received byte is going to be stored in"
* @return: Error codes if present
*/
ERROR_STATUS SPI_ReceiveByte(uint8 *ptru8_Data)
{
    if (ptru8_Data == NULL)
    {
        return E_NOK;
    }
    *ptru8_Data = gu8_RecievedData;
    if (gu8_RecievedFlag == FALSE)
    {
        return E_NOK;
    }
    else
    {
        gu8_RecievedFlag = FALSE;
        *ptru8_Data = gu8_RecievedData;
        return E_OK;
    }
    return E_OK;
}

/**
* @brief: Get status of SPI interrupt flag
* @param:
* Input : None
* Output: *u8_Data "points to where flag value is going to be stored in"
* @return: Error codes if present
*/
ERROR_STATUS SPI_GetStatus(uint8 *u8_Data)
{
    if (u8_Data == NULL)
    {
        return E_NOK;
    }
    else if ((SPI_STATUS_REG & 0x80) == 0)
    {
        *u8_Data = FALSE;
    }
    else
    {
        *u8_Data = TRUE;
        //DIO_Write(GPIOB, PIN3, HIGH);
    }
    return E_OK;
}
