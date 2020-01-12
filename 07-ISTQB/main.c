#include <stdio.h>
#include <stdlib.h>

//Software Engineer Name:





/*
- Define your global variables if needed here:
actual_result
expected_result
- Please consider the memory optimization.
*/




/*
- Define your functional prototypes used here:
sum
unittest
- Please concider optimum Misra C when writing your functions.
*/
int sum(int number1, int number2)
{
	return (number1 + number2);
}


void sum_unitTest (int result, int expected)
{
	if (expected==result)
	{
		printf("passed");
	}
	else
	{
		printf("failed");
	}
}

int main()
{
//Please declare your initializations here.
	int result = sum(5,7);
	sum_unitTest(result, 12);


//Please define your infinite loop here.


//Please insert your screen visualization here.


//Please write an expression that takes 2 numbers from the user here (concider screen readability).


//Please write an expression that calls the two function, the activation logic function and the unit test function in sequence.



    return 0;
}


//Please write the functional description of the sum function here:


//Please write the functional description of the unittest function here:



