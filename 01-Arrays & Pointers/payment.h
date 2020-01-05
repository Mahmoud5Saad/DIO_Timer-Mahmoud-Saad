/******************************************************************************
 *
 *	File Name: payment.h
 *
 * 	Description: header file for payment project
 *
 * 	Authors: Mahmoud Saad
 *
 *	Created on: Dec 2, 2019
 *
 *******************************************************************************/


#ifndef PAYMENT_H_
#define PAYMENT_H_

/*- INCLUDES -----------------------------------------------*/
#include <stdio.h>
#include <string.h>
/*- CONSTANTS ----------------------------------------------*/
/*- PRIMITIVE TYPES ----------------------------------------*/

/*- ENUMS --------------------------------------------------*/

typedef enum EN_transStat
{
	DECLINED,
	APPROVED
}EN_transStat;


/*- STRUCTS AND UNIONS -------------------------------------*/
typedef struct ST_cardData
{
	unsigned char cardHolderName[25];
	unsigned char primaryAccountNumber[19];
	unsigned char cardExpirationDate[6];
}ST_cardData;

typedef struct ST_terminalData
{
	unsigned char transAmount[8];
	unsigned char maxTransAmount[8];
	unsigned char transactionDate[11];
}ST_terminalData;

typedef struct ST_serverData
{
	EN_transStat transactionStatus;
	unsigned int receiptReferenceNumber;
}ST_serverData;


typedef struct ST_transaction
{
	ST_cardData cardHolderData;
	ST_terminalData transData;
	ST_serverData transResponse;
}ST_transaction;

typedef struct ST_accountBalance
{
	unsigned char balance[8];
	unsigned char primaryAccountNumber[19];
}ST_accountBalance;


/*- FUNCTION-LIKE MACROS -----------------------------------*/
/*- FUNCTION DECLARATIONS ----------------------------------*/
void checkBalance(ST_transaction *transaction);
void saveTransactionIntoServer(ST_transaction *transaction);
void checkExpiryDate(ST_transaction *transaction);
void fillTerminalData(ST_transaction *transaction);
void checkAmount(ST_transaction *transaction);
void fillCardData(ST_transaction *transaction);

/*- GLOBAL EXTERN VARIABLES -------------------------------*/
extern ST_transaction gst_PaymentTransaction;

#endif /* PAYMENT_H_ */
