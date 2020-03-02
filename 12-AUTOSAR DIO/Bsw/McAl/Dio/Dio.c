/*
 *  File Name : Dio.c
 *  Created on: Feb 19, 2020
 *  Author:     Badawi,Saad,Ahmed
 */
/*===========================================================*
 *                      Includes                             *
 *========================================================== */
#include "Dio.h"
#include "Dio_MemMap.h"
/*===========================================================*
 *               GPIO REGISTERS                              *
 *========================================================== */
typedef struct
{
    uint32 GPIODATA[256];
    uint32 GPIODIR     ;
    uint32 GPIOIS      ;
    uint32 GPIOIBE     ;
    uint32 GPIOIEV     ;
    uint32 GPIOIM      ;
    uint32 GPIORIS     ;
    uint32 GPIOMIS     ;
    uint32 GPIOICR     ;
    uint32 GPIOAFSEL   ; /*offset : 0x420 -> 424 */

    uint8 Reserved_2[0xDC];
    uint32 GPIODR2R    ; /*offset : 0x500 */
    uint32 GPIODR4R    ;
    uint32 GPIODR8R    ;
    uint32 GPIOODR     ;
    uint32 GPIOPUR     ;
    uint32 GPIOPDR     ;
    uint32 GPIOSLR     ;
    uint32 GPIODEN     ;
    uint32 GPIOLOCK    ;
    uint32 GPIOCR      ;
    uint32 GPIOAMSEL   ;
    uint32 GPIOPCTL    ;
    uint32 GPIOADCCTL  ;
    uint32 GPIODMACTL  ;
}PORT_RegType;

/*===========================================================*
 *              Local Macro Definition                       *
 *========================================================== */

#define PORTA_BaseAddr     ((volatile PORT_RegType*)0x40004000)
#define PORTB_BaseAddr     ((volatile PORT_RegType*)0x40005000)
#define PORTC_BaseAddr     ((volatile PORT_RegType*)0x40006000)
#define PORTD_BaseAddr     ((volatile PORT_RegType*)0x40007000)
#define PORTE_BaseAddr     ((volatile PORT_RegType*)0x40024000)
#define PORTF_BaseAddr     ((volatile PORT_RegType*)0x40025000)

#define DIO_DATA_FULL_PORT (255U)
#define MAX_NUM_OF_CH_IN_PORT       ((Dio_ChannelType)8U)
#define MAX_NUM_OF_PORTS            ((Dio_PortType)6U)

/*===========================================================*
 *                    Static Global Variables                *
 *========================================================== */
static volatile PORT_RegType* aPtrStr_BaseAddrArr[MAX_NUM_OF_PORTS] = {PORTA_BaseAddr,PORTB_BaseAddr,
                                                                       PORTC_BaseAddr,PORTD_BaseAddr,
                                                                       PORTE_BaseAddr,PORTF_BaseAddr};
/*===========================================================*
 *                    APIs Definitions                       *
 *========================================================== */

/*****************************************************************************************/
/*    Function Description    :  Returns the value of the specified DIO channel.
/*    Parameter in            :  ChannelId     : required Channel id to be read.
/*    Parameter inout         :  NA
/*    Parameter out           :  NA
/*    Return value            :  Dio_LevelType :
/*                                              - STD_HIGH The physical level of the
/*                                                corresponding Pin is STD_HIGH.
/*                                              - STD_LOW The physical level of the
/*                                                corresponding Pin is STD_LOW.
/*****************************************************************************************/
Dio_LevelType Dio_ReadChannel ( Dio_ChannelType ChannelId )
{
    Dio_LevelType ret_Dio_ReadChannel;
    Dio_PortType PortId_Dio_ReadChannel = (Dio_PortType)(ChannelId / MAX_NUM_OF_CH_IN_PORT);
    Dio_ChannelType ChannelPos = (Dio_ChannelType)(ChannelId % MAX_NUM_OF_CH_IN_PORT);
    volatile PORT_RegType* BaseAddr_Dio_ReadChannel;
    BaseAddr_Dio_ReadChannel = aPtrStr_BaseAddrArr[PortId_Dio_ReadChannel];
    ret_Dio_ReadChannel = (Dio_LevelType)((BaseAddr_Dio_ReadChannel->GPIODATA[(uint8)((uint8)(1)<<ChannelPos)])>>ChannelPos);
    return ret_Dio_ReadChannel;
}

/*****************************************************************************************/
/*    Function Description    :  Service to set a level of a channel.
/*    Parameter in            :  ChannelId : required Channel id to be written.
/*                               Level     : Value to be written.
/*    Parameter inout         :  NA
/*    Parameter out           :  NA
/*    Return value            :  NA
/******************************* **********************************************************/
void Dio_WriteChannel ( Dio_ChannelType ChannelId, Dio_LevelType Level )
{
    Dio_PortType PortId_Dio_WriteChannel = (Dio_PortType)(ChannelId / MAX_NUM_OF_CH_IN_PORT);
    Dio_ChannelType ChannelPos_Dio_WriteChannel = (Dio_ChannelType)(ChannelId % MAX_NUM_OF_CH_IN_PORT);
    volatile PORT_RegType* BaseAddr_Dio_WriteChannel;
    BaseAddr_Dio_WriteChannel = aPtrStr_BaseAddrArr[PortId_Dio_WriteChannel];
    uint32 temp_Dio_WriteChannel = (uint8)(Level<<ChannelPos_Dio_WriteChannel);
    BaseAddr_Dio_WriteChannel->GPIODATA[(uint8)((uint8)(1)<<ChannelPos_Dio_WriteChannel)] = (uint32)temp_Dio_WriteChannel;
}

/*****************************************************************************************/
/*    Function Description    : Service to flip (change from 1 to 0 or from 0 to 1) the
/*                              level of a channel and return the level of the channel after flip.
/*    Parameter in            : ChannelId : required Channel id to be written.
/*    Parameter inout         : NA
/*    Parameter out           : NA
/*    Return value            : Dio_LevelType :
/*                               - STD_HIGH The physical level of the
/*                                 corresponding Pin is STD_HIGH.
/*                               - STD_LOW The physical level of the
/*                                 corresponding Pin is STD_LOW.
/*****************************************************************************************/
Dio_LevelType Dio_FlipChannel ( Dio_ChannelType ChannelId )
{
    Dio_PortType PortId_Dio_FlipChannel = (Dio_PortType)(ChannelId / MAX_NUM_OF_CH_IN_PORT);
    Dio_ChannelType ChannelPos_Dio_FlipChannel = (Dio_ChannelType)(ChannelId % MAX_NUM_OF_CH_IN_PORT);
    volatile PORT_RegType* BaseAddr_Dio_FlipChannel;
    BaseAddr_Dio_FlipChannel = aPtrStr_BaseAddrArr[PortId_Dio_FlipChannel];
    BaseAddr_Dio_FlipChannel->GPIODATA[(uint8)((uint8)(0x01)<<(uint8)(ChannelPos_Dio_FlipChannel))] = (uint32)(~(BaseAddr_Dio_FlipChannel->GPIODATA[(uint8)((uint8)(1)<<(uint8)(ChannelPos_Dio_FlipChannel))]));
    Dio_LevelType ret_Dio_FlipChannel = BaseAddr_Dio_FlipChannel->GPIODATA[(uint8)((uint8)(0x01)<<(uint8)(ChannelPos_Dio_FlipChannel))]>>(uint8)(ChannelPos_Dio_FlipChannel);
    return ret_Dio_FlipChannel;
}

/*****************************************************************************************/
/*    Function Description    :  Returns the level of all channels of that port.
/*    Parameter in            :  PortId        : ID of DIO Port.
/*    Parameter inout         :  NA
/*    Parameter out           :  NA
/*    Return value            :  Dio_LevelType : Level of all channels of that port.
/*
/*****************************************************************************************/
Dio_PortLevelType Dio_ReadPort ( Dio_PortType PortId )
{
    Dio_PortLevelType Dio_PortLevel =  aPtrStr_BaseAddrArr[PortId]->GPIODATA[DIO_DATA_FULL_PORT];
    return  Dio_PortLevel;
}

/*****************************************************************************************/
/*    Function Description    :  Service to set a value of the port.
/*    Parameter in            :  ChannelId : ID of DIO Port.
/*                               Level     : Value to be written.
/*    Parameter inout         :  NA
/*    Parameter out           :  NA
/*    Return value            :  NA
/*****************************************************************************************/
void Dio_WritePort ( Dio_PortType PortId, Dio_PortLevelType Level )
{
    aPtrStr_BaseAddrArr[PortId]->GPIODATA[DIO_DATA_FULL_PORT]=Level;
}

/*****************************************************************************************/
/*    Function Description    :  This Service reads a subset of the adjoining bits of a port.
/*    Parameter in            :  Dio_ChannelGroupType: Pointer to ChannelGroup.
/*    Parameter inout         :  NA
/*    Parameter out           :  NA
/*    Return value            :  Dio_LevelType : Level of a subset of the adjoining bits of a port.
/*****************************************************************************************/
Dio_PortLevelType Dio_ReadChannelGroup ( const Dio_ChannelGroupType* ChannelGroupIdPtr )
{
    uint8 u8_mask = ChannelGroupIdPtr->mask;
    uint8 u8_offset = ChannelGroupIdPtr->offset;
    Dio_PortType Dio_PortType_port = ChannelGroupIdPtr->port;
    Dio_PortLevelType Dio_ChannelGroupLevel = (aPtrStr_BaseAddrArr[Dio_PortType_port]->GPIODATA[u8_mask])>>u8_offset;
    return Dio_ChannelGroupLevel;
}
/*****************************************************************************************/
/*    Function Description    :  Service to set a subset of the adjoining bits of a port
 *                               to a specified level.
/*    Parameter in            :  Dio_ChannelGroupType : Pointer to ChannelGroup.
/*                               Level     : Value to be written.
/*    Parameter inout         :  NA
/*    Parameter out           :  NA
/*    Return value            :  NA
/******************************* **********************************************************/
void Dio_WriteChannelGroup ( const Dio_ChannelGroupType* ChannelGroupIdPtr, Dio_PortLevelType Level )
{
    uint8 u8_mask = ChannelGroupIdPtr->mask;
    uint8 u8_offset = ChannelGroupIdPtr->offset;
    Dio_PortType Dio_PortType_port = ChannelGroupIdPtr->port;
    aPtrStr_BaseAddrArr[Dio_PortType_port]->GPIODATA[u8_mask] = Level<<u8_offset;
}


