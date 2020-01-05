/******************************************************************************
 *
 *	File Name: payment.c
 *
 * 	Description: source file for payment project
 *
 * 	Authors: Mahmoud Saad
 *
 *	Created on: Dec 2, 2019
 *
 *******************************************************************************/


/*- INCLUDES ----------------------------------------------*/
#include "payment.h"
/*- LOCAL MACROS ------------------------------------------*/
#define NOT_FOUND 	0
#define FOUND		1
#define NUMBER_OF_CUSTOMERS 10
#define CARD_NUMBER_LENGTH 9
/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/
/*- GLOBAL STATIC VARIABLES -------------------------------*/
/*- GLOBAL EXTERN VARIABLES -------------------------------*/
ST_transaction gst_PaymentTransaction;
ST_serverData g_ServerArray[256];
ST_accountBalance g_PrimaryAccountNumbers[NUMBER_OF_CUSTOMERS] = \
		{\
	{"0100.00", "123456789"},\
	{"6000.00", "234567891"},\
	{"3250.25", "567891234"},\
	{"1500.12", "456789123"},\
	{"0500.00", "258649173"},\
	{"2100.00", "654823719"},\
	{"0000.00", "971362485"},\
	{"0001.00", "793148625"},\
	{"0010.12", "123123456"},\
	{"0000.55", "456789321"}
		};
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/
/*- APIs IMPLEMENTATION -----------------------------------*/

void fillCardData(ST_transaction *transaction)
{
	ST_cardData pCard  = transaction->cardHolderData;
	printf("Please enter card Data:\n");
	printf("Please Enter the Card Holder Name:\n");
	fflush(stdin);
	fflush(stdout);
	gets((char*)(pCard.cardHolderName));
	printf("Please Enter the Primary Account Number:\n");
	fflush(stdin);
	fflush(stdout);
	gets((char*)(pCard.primaryAccountNumber));
	printf("Please card Expiry Date:\n");
	fflush(stdin);
	fflush(stdout);
	scanf("%s",(char*)(pCard.cardExpirationDate));
	fflush(stdin);
	fflush(stdout);
}

void fillTerminalData(ST_transaction *transaction)
{
	ST_terminalData pTerminal = transaction->transData;
	printf("Please Enter Terminal Data:\n");
	printf("Please Enter the transaction Amount:\n");
	fflush(stdin);
	fflush(stdout);
	gets((char*) (pTerminal.transAmount));
	printf("Please Enter transaction Date:\n");
	fflush(stdin);
	fflush(stdout);
	scanf("%s",((char*) (pTerminal.transactionDate)));
	fflush(stdin);
	fflush(stdout);
	printf("%s\n",pTerminal.transactionDate );
}


void checkBalance(ST_transaction *transaction)
{
	int i;
	int accountIndex = 0xFF;
	for (i=0; i<NUMBER_OF_CUSTOMERS; i++)
	{
		if ((strcmp( ((const char*)(transaction->cardHolderData).primaryAccountNumber) , ((const char*)((g_PrimaryAccountNumbers[i]).primaryAccountNumber) ))) == 0)
		{
			accountIndex = i;
		}
	}
	if ( accountIndex == 0xFF )
	{
		transaction->transResponse.transactionStatus =  APPROVED;
		printf("The Transaction is APPROVED");
	}
	else
	{
		for(i=0; i<7; i++)
		{
			if (((transaction->transData).transAmount)[i] > (g_PrimaryAccountNumbers[accountIndex].balance)[i])
			{
				transaction->transResponse.transactionStatus =  DECLINED;
				printf("The transaction is DECLINED");
				printf("balance");
				return;
			}
		}
	}
}

void checkAmount(ST_transaction *transaction)
{
	if ( (strcmp((char*)((transaction->transData).transAmount), (char*)((transaction->transData).maxTransAmount)) ) > 0)
	{
		transaction->transResponse.transactionStatus =  DECLINED;
		printf("The transaction is DECLINED");
		printf("Amount");
	}
}

void checkExpiryDate(ST_transaction *transaction)
{
	int i;
	int locationsCard[4] = {3,4,0,1};
	int locationsDate[4] = {8,9,3,4};
	unsigned char date[5] = {0, 0, 0, 0, '\0'};
	unsigned char card[5] = {0, 0, 0, 0, '\0'};
	printf("%s %s \n\n", date, card);

	for (i=0; i<4; i++)
	{
		date[i] = ((transaction->transData).transactionDate)[(locationsDate[i])];
		card[i] = (((transaction->cardHolderData).cardExpirationDate)[(locationsCard[i])]);
	}
	printf("%s %s \n\n", transaction->transData.transactionDate, transaction->cardHolderData.cardExpirationDate);
	printf("%s %s \n\n", date, card);

	if ( (strcmp((char*)date,(char*)card)) > 1 )
	{
		transaction->transResponse.transactionStatus =  DECLINED;
		printf("The transaction is DECLINED\n");
		printf("Expiry");
		return;
	}
}

void saveTransactionIntoServer(ST_transaction *transaction)
{
	static int counter = 0;
	printf("Verifying Data from the server…\n");
	checkExpiryDate(transaction);
	checkBalance(transaction);
	g_ServerArray[counter].receiptReferenceNumber = counter;
	g_ServerArray[counter].transactionStatus = transaction->transResponse.transactionStatus;
	counter++;
}

