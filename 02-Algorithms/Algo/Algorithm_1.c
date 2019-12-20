/* *****************************************************************
 *  Algorithm_1.c
 *
 *  Author: Ahmed Nofal, Instructor at Sprints LLC.
 ******************************************************************/

/******************************************************************
 * Given an array of integers, return indices of
 * the two numbers such that they add up to a specific target.
 * You may assume that each input would have exactly one solution,
 * and you may not use the same element twice.
 ***********
 * example *
 ***********
 * 		input: [10,2,23,4,5], target = 6
 * 		output: [1,3]
 *
 * *****************************************************************/

int* twoSum(int* nums, int numsSize, int target, int* returnSize)
{
	int i, j;
	static int elements[2];
	*returnSize = 2;
	for (i=0; i<numsSize; i++)
	{
		for (j=i;j<numsSize; j++)
		{
			if ( nums[i] + nums[j] == target)
			{
				elements[0] = i;
				elements[1] = j;
				return elements;
			}
		}
	}
	return NULL;
}
