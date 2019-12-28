/******************************************************************************
 *
 * 	Module: 		UART	
 *
 *	File Name: 		Uart.c
 *
 * 	Description: 	source code for UART
 *
 * 	Authors: 		Mahmoud Saad
 * 	
 *	Created on:		Saturday, 28-Dec-19 9:33:49 PM
 *
 *******************************************************************************/


/*******************************************************************************
 *			INCLUDES                                    
 *******************************************************************************/
#include "UART.h"
/*******************************************************************************
 *			Preprocessor Macros                                    
 *******************************************************************************/
/*******************************************************************************
 *			Enumerations and structures declarations                                  
 *******************************************************************************/
typedef enum Uart_state
{
	UartState_UNINIT, UartState_INIT
}Uart_state;
/*******************************************************************************
 *			Functions Prototypes                                   
 *******************************************************************************/
/*******************************************************************************
 *			Private functions Prototypes                                   
 *******************************************************************************/
/*******************************************************************************
 *			Global variables
 *******************************************************************************/
Uart_state ge_Uart_state = UartState_UNINIT;
void (*gpf_Transmit_Cbk_Uart)(void);
void (*gpf_Recieve_Cbk_Uart)(void);
void (*gpf_RegisterEmpty_Cbk_Uart)(void);
/*******************************************************************************
 *			Static global variables
 *******************************************************************************/
/*******************************************************************************
 *			Extern Global variables
 *******************************************************************************/
 /*******************************************************************************
*			Functions Definitions                                  
 *******************************************************************************/
 /** 
 * \brief UART_Init : 
 *
 * \param *pUART_cfg_s : A pointer to a struct that holds the desired UART configuration
 * \return ERROR_STATUS : Indication to the function execution 
 *
 **/     
ERROR_STATUS UART_Init(UART_cfg_s *pUART_cfg_s)
{
	if (	(NULL == pUART_cfg_s) || !(UartState_UNINIT == ge_Uart_state) ||\
			(NULL==pUART_cfg_s->DataRegisterEmpty_Cbk_ptr) ||\
			(NULL==pUART_cfg_s->Resceive_Cbk_ptr) ||\
			(NULL==pUART_cfg_s->Transmit_Cbk_ptr) )
	{
		return E_NOK;
	} 
	else
	{
		if (UART_INTERRUPT == pUART_cfg_s->u8_InterruptMode)
		{
			/* enable interrupts */
			/* store global pointers */
			if (NULL != pUART_cfg_s->Transmit_Cbk_ptr)
			{
				gpf_Transmit_Cbk_Uart = pUART_cfg_s->Transmit_Cbk_ptr;
			}
			if (NULL != pUART_cfg_s->Resceive_Cbk_ptr)
			{
				gpf_Recieve_Cbk_Uart = pUART_cfg_s->Resceive_Cbk_ptr;
			}
			if (NULL != pUART_cfg_s->DataRegisterEmpty_Cbk_ptr)
			{
				gpf_RegisterEmpty_Cbk_Uart = pUART_cfg_s->DataRegisterEmpty_Cbk_ptr;
			}
		}
		switch (pUART_cfg_s->u8_DesiredOperation)
		{
			case TRANSMITTER:
				UART_CSRB |= UART_TXEN;
				break;
			case RECEIVER:
				UART_CSRB |= UART_RXEN;
				break;
			case TRANSCEIVER:
				UART_CSRB |= UART_TXEN | UART_RXEN;
				break;
			default:
				return E_NOK;
		}
		if (UART_DOUBLE_SPEED == pUART_cfg_s->u8_DoubleSpeed)
		{
			UART_CSRA |= UART_U2X;
		}
		if (UART_TWO_STOP_BIT == pUART_cfg_s->u8_StopBit)
		{
			UART_CSRC |= UART_SBS;
		}
		switch (pUART_cfg_s->u8_ParityBit)
		{
			case UART_EVEN_PARITY:
				UART_CSRC |= UART_PM1;
				break;
			case UART_ODD_PARITY:
				UART_CSRC |= UART_PM1 | UART_PM0;
				break;
			default:
				return E_NOK;
				break;
		}
		switch (pUART_cfg_s->u8_DataSize)
		{
			/* 8-bit is default */
			case UART_5_BIT:
				break;
			case UART_6_BIT:
				break;
			case UART_7_BIT:
				break;
			case UART_8_BIT:
				break;
			case UART_9_BIT:
				break;													
		}
		/* get 9600 baudrate */
		UART_BRRH = 0;
		if (UART_DOUBLE_SPEED == pUART_cfg_s->u8_DoubleSpeed)
		{
			UART_BRRL = 12;
		} 
		else if (UART_NO_DOUBLE_SPEED == pUART_cfg_s->u8_DoubleSpeed)
		{
			UART_BRRL = 12;
		}
		ge_Uart_state = UartState_INIT;
		return E_OK;
	}
}

/** 
 * \brief UART_Deinit : Deinitialize the UART module
 *
 * \param void
 * \return ERROR_STATUS : Indication to the function execution 
 *
 **/     
ERROR_STATUS UART_Deinit(void);

/** 
 * \brief  UART_SendByte : Sends a certain byte
 *
 * \param u8_Data : The byte the user wants to send
 * \return ERROR_STATUS : Indication to the function execution 
 *
 **/  
ERROR_STATUS UART_SendByte(uint8 u8_Data)
{
	UART_DR = u8_Data;
	while (UART_CSRA&UART_TXC);
	UART_CSRA |= UART_TXC;
	return E_OK;
}

/** 
 * \brief  UART_ReceiveByte : Receives a certain byte
 *
 * \param pu8_ReceivedData : The sent byte will be stored in the passed pointer 
 * \return ERROR_STATUS : Indication to the function execution 
 *
 **/  
ERROR_STATUS UART_ReceiveByte(uint8 *pu8_ReceivedData)
{
	while (UART_CSRA&UART_TXC);
	*pu8_ReceivedData = UART_DR;
	return E_OK;
}