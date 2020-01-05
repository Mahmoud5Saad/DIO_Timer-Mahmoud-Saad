/* *****************************************************************
 *  Algorithm_3.c
 *
 *  Author: Ahmed Nofal, Instructor at Sprints LLC.
 ******************************************************************/

 
 #define POWER_TO_FIVE	5U
 
/*********************************************************************************
*	Given a number, print floor of 5’th root of the number.
*	
*	Input  : n = 32
*	Output : 2
*	2 raise to power 5 is 32
*
*	Input  : n = 250
*	Output : 3
*	Fifth square root of 250 is between 3 and 4
*	So floor value is 3.
********************************************************************************/ 


int Linear_floorRoot5(int n)
{
	int trial = 1;
	int result = -1;
	while(trial!=n)
	{
		trial++;
		if( n == (trial*trial*trial*trial*trial) )
		{
			result = trial;
			break;
		}
		else if ( (trial*trial*trial*trial*trial) > n )
		{
			trial--;
			result = trial;
			break;
		}
	}
	return result;
}

