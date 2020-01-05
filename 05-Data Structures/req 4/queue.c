/******************************************************************************
 *
 * 	Module:
 *
 *	File Name: queue.c
 *
 * 	Description:
 *
 * 	Authors: Mahmoud Saad
 *
 *	Created on: Dec 19, 2019
 *
 *******************************************************************************/

/*- INCLUDES ----------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
/*- LOCAL MACROS ------------------------------------------*/
/*- LOCAL FUNCTIONS PROTOTYPES ----------------------------*/
/*- GLOBAL STATIC VARIABLES -------------------------------*/
/*- GLOBAL EXTERN VARIABLES -------------------------------*/
/*- LOCAL FUNCTIONS IMPLEMENTATION ------------------------*/
/*- APIs IMPLEMENTATION -----------------------------------*/

/*******************************************************************************
 *			Preprocessor Macros
 *******************************************************************************/

/*******************************************************************************
 *			Enumerations and structures declarations
 *******************************************************************************/
typedef struct ST_queueInfo
{
	int* g_base;
	int* g_frontQueue;
	int* g_backQueue;
	int queue_size;
}ST_queueInfo;

/*******************************************************************************
 *			Functions Prototypes
 *******************************************************************************/

/*******************************************************************************
 *			Private functions Prototypes
 *******************************************************************************/
void createQueue(ST_queueInfo* info, int maxSize);
void enqueue(ST_queueInfo *info, int data);
void dequeue(ST_queueInfo *info, int* data);
long long evaluate(char* expression);
/*******************************************************************************
 *			Global variables
 *******************************************************************************/
/*******************************************************************************
 *			Static global variables
 *******************************************************************************/
/*******************************************************************************
 *			Extern Global variables
 *******************************************************************************/
/*******************************************************************************
 *			Functions Definitions
 *******************************************************************************/

void createQueue(ST_queueInfo* info, int maxSize)
{
	int *front = (int*)malloc (sizeof(int)*maxSize);
	if ( NULL == front )
	{
		return;
	}
	else
	{
		info->g_base = front;
		info->g_frontQueue = front;
		info->g_backQueue = front;
		info->queue_size = maxSize;
	}
}
void enqueue(ST_queueInfo *info, int data)
{
	*(info->g_frontQueue) = data;
	(info->g_frontQueue)++;
}
void dequeue(ST_queueInfo *info, int* data)
{
	*data = *(info->g_backQueue);
	(info->g_backQueue)++;
}

long long evaluate(char* expression)
{
	int i = 0, j = 0;
	ST_queueInfo operationQueue;
	ST_queueInfo termsQueue;
	int operand;
	int operation;
	long long result;
	char buffer[20] = {0};
	createQueue(&operationQueue, 50);
	createQueue(&termsQueue, 50);
	/* put operands and operations in queues */
	while ( '\0' != expression[i] )
	{
		switch( expression[i] )
		{
		case	'+':
		case	'-':
		case	'*':
		case	'/':
			enqueue(&operationQueue, expression[i]);
			i++;
			break;
		default:
			if ( expression[i] >= '0' && expression[i] <= '9' )
			{
				j = 0;
				while ( expression[i] >= '0' && expression[i] <= '9')
				{
					buffer[j] = expression[i];
					i++;
					j++;
				}
				buffer[j] = '\0';
				operand = atoi((const char *) buffer);
				enqueue(&termsQueue, operand);
			}
			else
			{
				i++;
			}
			break;
		}
	}
	/* dequeue first operand and store in result */
	dequeue(&termsQueue, (int *)&result);
	/* evaluate expression */
	while ( operationQueue.g_frontQueue != operationQueue.g_backQueue )
	{
		dequeue(&termsQueue, &operand);
		dequeue(&operationQueue, &operation);
		switch(operation)
		{
		case	'+':
			result += operand;
			break;
		case	'-':
			result -= operand;
			break;
		case	'*':
			result *= operand;
			break;
		case	'/':
			result /= operand;
			break;
		default:
			break;
		}
	}
	return result;
}

int main (void)
{
	char* expression0 = "(50+8000)-((30/10))";
	printf("%d", (int) evaluate(expression0));
	return 0;
}
