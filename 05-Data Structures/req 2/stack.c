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
#define 	BALANCED				(1U)
#define		NOT_BALANCED			(0U)
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
unsigned char checkForBalancedParantheses(char* expression);
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

unsigned char checkForBalancedParantheses(char* expression)
{
	int returnFlag = BALANCED;
	int i = 0;
	int term = 0;
	ST_stackInfo brackets = { NULL, NULL, 0 };
	createStack(&brackets,1000);
	/* stack all right facing brackets from left side */
	while ('\0' != expression[i] )
	{
		switch (expression[i])
		{
		case '[':
		case '{':
		case '(':
			push(&brackets, (int)expression[i]);
			break;
		case ']':
			if ( brackets.g_base == brackets.g_firstEmpty )
			{
				return NOT_BALANCED;
			}
			else
			{
				pop(&brackets, &term);
				if (term != '[')
				{
					return NOT_BALANCED;
				}
				else{}
			}
			break;
		case '}':
			if ( brackets.g_base == brackets.g_firstEmpty )
			{
				return NOT_BALANCED;
			}
			else
			{
				pop(&brackets, &term);
				if (term != '{')
				{
					return NOT_BALANCED;
				}
				else{}
			}
			break;
		case ')':
			if ( brackets.g_base == brackets.g_firstEmpty )
			{
				return NOT_BALANCED;
			}
			else
			{
				pop(&brackets, &term);
				if (term != '(')
				{
					return NOT_BALANCED;
				}
				else{}
			}
			break;
		default:
			break;
		}
		i++;
	}
	if ( brackets.g_base != brackets.g_firstEmpty )
	{
		return NOT_BALANCED;
	}
	return returnFlag;
}

/* testing */

int main (void)
{
	char * str = "Saad(soliman)";
	char * str0 = "Saad(soliman)[]{}";
	char * str1 = "Saad({soliman})";
	char * str2 = "Saad(soliman)[{}]";
	char * str3 = "(P{}";
	char * str4 = "({)}";
	char * str5 = "Saad({soliman)}";
	printf("Should pass:\n%s\t\t%d\n", str, checkForBalancedParantheses(str));
	printf("%s\t%d\n", str0, checkForBalancedParantheses(str0));
	printf("%s\t\t%d\n", str1, checkForBalancedParantheses(str1));
	printf("%s\t%d\n", str2, checkForBalancedParantheses(str2));
	printf("Should Fail:\n%s\t\t\t%d\n", str3, checkForBalancedParantheses(str3));
	printf("%s\t\t\t%d\n", str4, checkForBalancedParantheses(str4));
	printf("%s\t\t%d\n", str5, checkForBalancedParantheses(str5));
	return 0;
}
