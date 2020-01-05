/******************************************************************************
 *
 * 	Module:
 *
 *	File Name: stack.c
 *
 * 	Description: implementation of stack
 *
 * 	Authors: Mahmoud Saad
 *
 *	Created on: 17-Dec-2019
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
 *			Functions Prototypes
 *******************************************************************************/

/*******************************************************************************
 *			Functions Definitions
 *******************************************************************************/
/*******************************************************************************
 *			Enumerations and structures declarations
 *******************************************************************************/
typedef struct ST_stackInfo
{
	int* g_base;
	int* g_firstEmpty;
	int g_size;
}ST_stackInfo;
/*******************************************************************************
 *			Private functions Prototypes
 *******************************************************************************/
void createStack(ST_stackInfo *info, int size);
void push(ST_stackInfo *info, int data);
void pop(ST_stackInfo *info, int* data);
/*******************************************************************************
 *			Global variables
 *******************************************************************************/
ST_stackInfo stack1 = { NULL, NULL, 0 };
/*******************************************************************************
 *			Static global variables
 *******************************************************************************/
/*******************************************************************************
 *			Extern Global variables
 *******************************************************************************/





void createStack(ST_stackInfo *info, int size)
{
	int* base;
	base = (int*)malloc(sizeof(int)*size);
	if ( NULL == base )
	{
		return;
	}
	info->g_size = size;
	info->g_base = base;
	info->g_firstEmpty = base;
}

void push(ST_stackInfo *info, int data)
{
	*(info->g_firstEmpty) = data;
	(info->g_firstEmpty)++;
}

void pop(ST_stackInfo *info, int* data)
{
	(info->g_firstEmpty)--;
	*data = *(info->g_firstEmpty);
}

/* testing */
ST_stackInfo stack_1 = { NULL, NULL, 0 };
int main (void)
{
	int read;
	createStack(&stack_1, 5);
	push(&stack_1, 40);
	push(&stack_1, 50);
	push(&stack_1, 60);
	push(&stack_1, 70);
	push(&stack_1, 80);
	pop(&stack_1, &read);
	printf("%d\n", read);
	pop(&stack_1, &read);
	printf("%d\n", read);
	pop(&stack_1, &read);
	printf("%d\n", read);
	pop(&stack_1, &read);
	printf("%d\n", read);
	pop(&stack_1, &read);
	printf("%d\n", read);
	return 0;
}
