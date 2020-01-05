/*********************************************************************************
 *
 *  problem_three.c
 *  Author: Ahmed Nofal, Instructor at Sprints LLC
 *
 ********************************************************************************/

/****************************[Problem Description]*********************************
 *
 * Count occurances of a number in a sorted array
 * Sample Input: arr= [2,2, 4, 5, 5, 5, 5, 6, 6]
 * 				 target = 5
 * Sample Output: The number 5 occured 4 times in the array
 *
 ********************************************************************************/
#include <stdio.h>
#include "problem_three.h"


/*--------------------MACROS--------------------*/
#define FIRST_OCCURENCE 	1U
#define LAST_OCCURENCE 		0U




/*********************************************************************************
 *
 * Function: BinarySearchOccurances
 *
 * @param: arr[], Array that the function should find the number in.
 * @param: arr_size, Size of the array
 * @param: target, The elment to be searched for the last occurance for
 * @param: searchFirst, Indicates whether you are using this function to get the
 * first occurance of the target or last (1 for first and 0 for last)
 * @return: int, the index of the last occurance of the element
 *
 * Description: function to get the number of occurances of a target element in a
 * given sorted array.
 *
 *
 ********************************************************************************/
int BinarySearchOccurances(int arr[], int arr_size, int target, int searchFirst){
	int i=0;
	int result = -1;
	int f_notFound = 1;
	int middle = arr_size/2;
	int shift = arr_size/4;
	switch (searchFirst)
	{
	case FIRST_OCCURENCE:
		while (f_notFound)
		{
			if (target == arr[middle])
			{
				result = middle;
				f_notFound = 0;
				while(arr[result-i]==arr[middle])
				{
					i++;
				}
				result = middle - i + 1;
			}
			else if(target < arr[middle])
			{
				middle -= shift;
			}
			else if (target > arr[middle])
			{
				middle += shift;
			}
			if (shift == 0)
			{
				return -1;
			}
			shift/=2;
		}
		break;
	case LAST_OCCURENCE:
		while (f_notFound)
		{
			if (target == arr[middle])
			{
				result = middle;
				f_notFound = 0;
				while(arr[result+i]==arr[middle])
				{
					i++;
				}
				result = middle + i - 1;
			}
			else if(target < arr[middle])
			{
				middle -= shift;
			}
			else if (target > arr[middle])
			{
				middle += shift;
			}
			if (shift == 0)
			{
				return -1;
			}
			shift/=2;
		}
		break;
	}
	return result;
}

/* Count occurrences of a number in a sorted array with duplicates*/
void BinarySearchOccurances_test(void)
{
	int A[] = {2, 5, 5, 5, 6, 6, 8, 9, 9, 9};
	int target = 5;

	int n = sizeof(A)/sizeof(A[0]);
	int first = BinarySearchOccurances(A, n, target, 1);  // 1 for first occurrence
	int last = BinarySearchOccurances(A, n, target, 0);   // 0 for last occurrence

	int count = last - first + 1;

	if (first != -1)
		printf("Element %d occurs %d times.", target, count);
	else
		printf("Element not found in the array.");
}

