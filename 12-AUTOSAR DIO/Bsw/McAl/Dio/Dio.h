
/*===========================================================*
 *                      Includes                             *
 *========================================================== */
#include "Dio_Cfg.h"
#include "Dio_Types.h"
/*============================================================================ *
 *                        Api.s Exports                                        *
 *============================================================================ */

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
extern Dio_LevelType Dio_ReadChannel ( Dio_ChannelType ChannelId );

/*****************************************************************************************/
/*    Function Description    :  Service to set a level of a channel.
/*    Parameter in            :  ChannelId : required Channel id to be written.
/*                               Level     : Value to be written.
/*    Parameter inout         :  NA
/*    Parameter out           :  NA
/*    Return value            :  NA
/******************************* **********************************************************/
extern void Dio_WriteChannel ( Dio_ChannelType ChannelId, Dio_LevelType Level );
/*****************************************************************************************/
/*    Function Description    :  Returns the level of all channels of that port.
/*    Parameter in            :  PortId        : ID of DIO Port.
/*    Parameter inout         :  NA
/*    Parameter out           :  NA
/*    Return value            :  Dio_LevelType : Level of all channels of that port.
/*
/*****************************************************************************************/
extern Dio_PortLevelType Dio_ReadPort ( Dio_PortType PortId );

/*****************************************************************************************/
/*    Function Description    :  Service to set a value of the port.
/*    Parameter in            :  ChannelId : ID of DIO Port.
/*                               Level     : Value to be written.
/*    Parameter inout         :  NA
/*    Parameter out           :  NA
/*    Return value            :  NA
/*****************************************************************************************/
extern void Dio_WritePort ( Dio_PortType PortId, Dio_PortLevelType Level );

/*****************************************************************************************/
/*    Function Description    :  This Service reads a subset of the adjoining bits of a port.
/*    Parameter in            :  Dio_ChannelGroupType: Pointer to ChannelGroup.
/*    Parameter inout         :  NA
/*    Parameter out           :  NA
/*    Return value            :  Dio_LevelType : Level of a subset of the adjoining bits of a port.
/*****************************************************************************************/
extern Dio_PortLevelType Dio_ReadChannelGroup ( const Dio_ChannelGroupType* ChannelGroupIdPtr );

/*****************************************************************************************/
/*    Function Description    :  Service to set a subset of the adjoining bits of a port
 *                               to a specified level.
/*    Parameter in            :  Dio_ChannelGroupType : Pointer to ChannelGroup.
/*                               Level     : Value to be written.
/*    Parameter inout         :  NA
/*    Parameter out           :  NA
/*    Return value            :  NA
/******************************* **********************************************************/
extern void Dio_WriteChannelGroup ( const Dio_ChannelGroupType* ChannelGroupIdPtr, Dio_PortLevelType Level );
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
extern Dio_LevelType Dio_FlipChannel ( Dio_ChannelType ChannelId );

