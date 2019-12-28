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
#include "registers.h"
 /************************************************************************/
/*			LOCAL MACROS/DEFINES FOR REGISTERS & PINS		        */
/************************************************************************/
#define SPI_DATA 	    *((reg_type8)(0x2F))
#define SPI_STATUS_REG  *((reg_type8)(0x2E))
#define SPI_CTRL_REG    *((reg_type8)(0x2D))

#define SPI_PORT    GPIOB
#define SS_PIN      PIN4
#define MOSI_PIN    PIN5
#define MISO_PIN    PIN6
#define SCK_PIN     PIN7
/************************************************************************/
/*			LOCAL MACROS/DEFINES FOR MASKS/VALUES/FUNCTIONS		        */
/************************************************************************/
#define SPI_INT_FLAG        0x80
#define SPI_INT_ENB         0x80
#define SPI_POLLING         0x7F
#define SPI_INT_MASK        0x7F
#define SPI_ENB             0x40
#define SPI_LSB_FIRST       0x20
#define SPI_MSB_FIRST       0xDF
#define SPI_ORDER_MASK      0xDF
#define SPI_MASTER_MODE     0x10
#define SPI_SLAVE_MODE      0xEF
#define SPI_MODE_MASK       0xEF
#define SPI_PRESCALER_MASK  0xFC
#define SPI_DATA_MODE_MASK  0xF3
#define STU_PRESCALER_MASK  0xF7


#define TRANSMITTING    0
#define BYTE_SENT       1

#define SET_SS_PIN_LOW()    DIO_Write(SPI_PORT, SS_PIN, LOW)

#define SET_SS_PIN_HIGH()   DIO_Write(SPI_PORT, SS_PIN, HIGH)

#define RETURN_ON_E_NOK(VAR){\
if ((VAR) == E_NOK)\
    {\
        return E_NOK;\
    }\
    else\
    {\
    }\
}
/************************************************************************/
/*		         GLOBAL STATIC VARIABLES		        */
/************************************************************************/
static uint8 gu8CheckInitialization = 0;
static uint8 Direction = SLAVE;
static void (*gfpSpiFunctionPointer) (void) = NULL;
static const uint8 gau8SpiMode[2] = {SPI_SLAVE_MODE, SPI_MASTER_MODE};
static const uint8 gau8SpiDataOrder[2] = {SPI_MSB_FIRST, SPI_LSB_FIRST};
static const uint8 gau8SpiPollingOrINT[2] = {SPI_POLLING, SPI_INT_ENB};
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
ISR(SPI_STC_vect){
    if (gfpSpiFunctionPointer != NULL)
    {
        (*gfpSpiFunctionPointer)();
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
    else if (pstr_SPI_Confg->CBK_Func == NULL)
    {
        return E_NOK;
    }
    else if (pstr_SPI_Confg->u8_DataMode > MODE_3)
    {
        return E_NOK;
    }
    else if (pstr_SPI_Confg->u8_DataOrder > LSB)
    {
        return E_NOK;
    }
    else if (pstr_SPI_Confg->u8_InterruptMode > INTERRUPT)
    {
        return E_NOK;
    }
    else if (pstr_SPI_Confg->u8_SPIMode > MASTER)
    {
        return E_NOK;
    }
    else if (pstr_SPI_Confg->u8_Prescaler > SPI_PRESCALER_128)
    {
        return E_NOK;
    }
    else
    {

    }

    DIO_Cfg_s SPI_PinConfig;
    SPI_PinConfig.GPIO = SPI_PORT;
    SPI_CTRL_REG = (SPI_CTRL_REG & SPI_DATA_MODE_MASK) | ((pstr_SPI_Confg->u8_DataMode) << 2);
    SPI_CTRL_REG = (SPI_CTRL_REG & SPI_PRESCALER_MASK) | ((pstr_SPI_Confg->u8_Prescaler) / 2);
    SPI_STATUS_REG = (SPI_STATUS_REG & STU_PRESCALER_MASK) | ((pstr_SPI_Confg->u8_Prescaler + 1) % 2);
    SPI_CTRL_REG = (SPI_CTRL_REG & SPI_ORDER_MASK) | gau8SpiDataOrder[pstr_SPI_Confg->u8_DataOrder];
    SPI_CTRL_REG = (SPI_CTRL_REG & SPI_INT_MASK) | gau8SpiPollingOrINT[pstr_SPI_Confg->u8_InterruptMode];
    SPI_CTRL_REG = (SPI_CTRL_REG & SPI_MODE_MASK) | gau8SpiMode[pstr_SPI_Confg->u8_SPIMode];

    if (pstr_SPI_Confg->u8_SPIMode == MASTER)
    {
        SPI_PinConfig.pins = (MOSI_PIN | SS_PIN | SCK_PIN);
        SPI_PinConfig.dir = OUTPUT;
        DIO_init(&SPI_PinConfig);
        SPI_PinConfig.pins = (MISO_PIN);
        SPI_PinConfig.dir = INPUT;
        DIO_init(&SPI_PinConfig);
        Direction = MASTER;
    }
    else if (pstr_SPI_Confg->u8_SPIMode == SLAVE)
    {
        SPI_PinConfig.pins = (MOSI_PIN | SS_PIN | SCK_PIN);
        SPI_PinConfig.dir = INPUT;
        DIO_init(&SPI_PinConfig);
        SPI_PinConfig.pins = (MISO_PIN);
        SPI_PinConfig.dir = OUTPUT;
        DIO_init(&SPI_PinConfig);
        Direction = SLAVE;
    }
    gfpSpiFunctionPointer = pstr_SPI_Confg->CBK_Func;
    gu8CheckInitialization = 1;
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
    if (gu8CheckInitialization == 0)
    {
        return E_NOK;
    }
    else
    {

    }
    if (Direction == MASTER)
    {
        SET_SS_PIN_LOW();
    }
    else
    {

    }
    SPI_DATA = (SPI_DATA & 0x00) | (uint8)(u8_Data);
    uint8 SendingStatus = TRANSMITTING;
    while (SendingStatus != BYTE_SENT)
    {
        SPI_GetStatus(&SendingStatus);
    }
    if (Direction == MASTER)
    {
        SET_SS_PIN_HIGH();
    }
    else
    {

    }
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
    if (gu8CheckInitialization == 0)
    {
        return E_NOK;
    }
    else if (ptru8_Data == NULL)
    {
        return E_NOK;
    }
    else
    {

    }
    if (Direction == MASTER)
    {
        SET_SS_PIN_LOW();
    }
    else
    {

    }
    uint8 RecievingStatus = TRANSMITTING;
    while (RecievingStatus != BYTE_SENT)
    {
        SPI_GetStatus(&RecievingStatus);
    }
    if (Direction == MASTER)
    {
        SET_SS_PIN_HIGH();
    }
    else
    {

    }
    *ptru8_Data = SPI_DATA;
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
    if (gu8CheckInitialization == 0)
    {
        return E_NOK;
    }
    else if (u8_Data == NULL)
    {
        return E_NOK;
    }
    else
    {

    }
    if ((SPI_STATUS_REG & SPI_INT_FLAG) == 0)
    {
        *u8_Data = TRANSMITTING;
    }
    else
    {
        *u8_Data = BYTE_SENT;
    }
    return E_OK;
}
