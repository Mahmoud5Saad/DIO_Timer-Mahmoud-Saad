/*********************************************************************************
 *
 *  problem_two.c
 *  Author: Ahmed Nofal, Instructor at Sprints LLC 
 *
 ********************************************************************************/


/****************************[Problem Description]*********************************
 *
 * Modify the Binary Search algorithm to find an element in a circularly sorted
 * array.
 * The array is sorted in an ascending order and rotated in clockwise
 *
 * Sample Input: arr = [8, 9, 10, 2, 5, 6]
 * 				 target = 10
 * Sample Output: The number is found at index 2
 *
 ********************************************************************************/
#include <stdio.h>
#include "problem_two.h"

/*********************************************************************************
 *
 * Function: circularArraySearch
 *
 * @param: arr[], Array that the function should find the number in.
 * @param: arr_size, Size of the array
 * @param: element_to_be_found, The elment to be searcher for
 * @return: int, the index of the element to be found
 *
 * Description: Function to find the number of times the array is rotated, if the
 * element was not found it should return -1
 *
 ********************************************************************************/
int circularArraySearch(int arr[], int arr_size, int element_to_be_found)
{
	int result = -1;
	int f_notFound = 1;
	int middle;
	int jump;
	int i;
	for(i=1; i<arr_size; i++)
	{
		/* if order breaks return where it broke as the rotation */
		if( arr[i-1] > arr[i] )
		{
			break;
		}
	}
	if (element_to_be_found == arr[0])
	{
		return 0;
	}
	else if ( element_to_be_found > arr[0])
	{
		arr_size = i;
		middle = arr_size/2;
		jump = arr_size/4;
	}
	else
	{
		arr_size = arr_size - 1;
		middle = arr_size/2;
		jump = middle/2;
		arr = arr + i;
	}
	while (f_notFound)
	{
		if (element_to_be_found == arr[middle])
		{
			result = middle;
			f_notFound = 0;
		}
		else if(element_to_be_found < arr[middle])
		{
			middle -= jump;
		}
		else if (element_to_be_found > arr[middle])
		{
			middle += jump;
		}
		if (jump == 0 && f_notFound == 1)
		{
			return -1;
		}
		jump/=2;
	}
	return result;
}

/* circularArraySearch_test */
void circularArraySearch_test(void)
{
	int A[] = {9, 10, 2, 5, 6, 8};
	int target = 10;
	int n = sizeof(A)/sizeof(A[0]);
	int index = circularArraySearch(A, n, target);
	if(index != -1){
		printf("Element found at index %d", index);
	}
	else{
		printf("Element not found in the array");
	}

}
