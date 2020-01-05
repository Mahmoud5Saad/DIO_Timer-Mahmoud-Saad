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

int main  (void)
{

}
