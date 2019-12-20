/* *****************************************************************
 *  Algorithm_2.c
 *
 *  Author: Ahmed Nofal, Instructor at Sprints LLC.
 ******************************************************************/
#include <stdio.h>

/*********************************************************************************
*	Given a number n, write a program to print a diamond shape with 2n rows.
*
*	Input: 5
*	Output: 
*				*
*			   * *
*			  * * *
*			 * * * *
*			* * * * *
*			* * * * *
*			 * * * *
*			  * * *
*			   * *
*				*
********************************************************************************/

void PrintDiamond(int n)
{
	int i, j;
	int middleLeft = n;
	int middleRight = n;
	/* nested loops */

	for(i=0; i<n; i++)
	{
		for(j=0; j<2*n;j++)
		{
			/* chooses star slots*/
			if((i%2==0 && j%2 == 1) || (i%2==1 && j%2 == 0))
			{
				/* choose slots to print */
				if ( j>= middleLeft && j<= middleRight)
				{
					printf("*");
				}
				else
				{
					printf(" ");
				}

			}
			else
			{
				printf(" ");
			}
		}
		middleLeft--;
		middleRight++;
		printf("\n");
	}
	middleLeft = 0;
	middleRight = 2*n;
	for(i=0; i<n; i++)
	{
		for(j=0; j<2*n;j++)
		{
			/* chooses star slots*/
			if((i%2==0 && j%2 == 1) || (i%2==1 && j%2 == 0))
			{
				/* choose slots to print */
				if ( j>= middleLeft && j<= middleRight)
				{
					printf("*");
				}
				else
				{
					printf(" ");
				}

			}
			else
			{
				printf(" ");
			}
		}
		middleLeft++;
		middleRight--;
		printf("\n");
	}
}


