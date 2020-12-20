
#include "test_functions.h"

/// ********this module contains functions that related to the test of data **********
//the the main thread generats numbers and write them to file, creates file of prioritization , and than calls
//the "HW3" function , with these files as it's input. the test file is "TasksTest.txt"


//inputs: priorities_file- handle to the prioritis file, test_priorities- array of priorities
//output:ret_val1- indicates if the function succeeded or not
//functionality : this function writes the priorities to the prioritis file.
int write_to_priorities_file(HANDLE priorities_file, int test_priorities[]) {
	int temp_num1, temp_num2, ret_val1;
	char temp_num_char1[2] = { '0','\0' };
	char temp_num_char2[2] = { '0','\0' };
	char temp_num_str[NUM_OF_CHARS_IN_NUM + 1] = { 0,0,0,0,0 };
	char temp_num_str_for_priorities[NUM_OF_CHARS_IN_NUM] = { 0,0,0,0 };
	for (int i = 0; i < NUM_OF_NUMERS_TO_TEST; i++) {
		if (test_priorities[i] < 10) {
			temp_num_str_for_priorities[0] = (test_priorities[i] % 10) + '0';
			temp_num_str_for_priorities[1] = '\r';
			temp_num_str_for_priorities[2] = '\n';
			temp_num_str_for_priorities[3] = '\0';
			ret_val1 = WriteFileWrap(priorities_file, temp_num_str_for_priorities, NUM_OF_CHARS_IN_NUM - 1);
			if (ret_val1 != SUCCESS)
			{
				CloseHandleWrap(priorities_file);
				return ret_val1;
			}
		}
		else {
			temp_num1 = test_priorities[i] / 10;
			temp_num2 = test_priorities[i] % 10;
			temp_num_char1[0] = temp_num1 + '0';
			temp_num_char2[0] = temp_num2 + '0';
			temp_num_str[0] = temp_num_char1[0];
			temp_num_str[1] = temp_num_char2[0];
			temp_num_str[2] = '\r';
			temp_num_str[3] = '\n';
			temp_num_str[4] = '\0';
			ret_val1 = WriteFileWrap(priorities_file, temp_num_str, NUM_OF_CHARS_IN_NUM);
			if (ret_val1 != SUCCESS)
			{
				CloseHandleWrap(priorities_file);
				return ret_val1;
			}
		}
	}
	return ret_val1;
}
//inputs: tasks_file- handle to the tasks file, test_array- array of tasks to preform
//output:ret_val1- indicates if the function succeeded or not
//functionality : this function writes the tasks to the tasks file.
int write_to_tasks_file(HANDLE tasks_file, int* test_array) {
	int temp_num1, temp_num2, ret_val1;
	char temp_num_char1[2] = { '0','\0' };
	char temp_num_char2[2] = { '0','\0' };
	char temp_num_str[NUM_OF_CHARS_IN_NUM + 1] = { 0,0,0,0,0 };
	for (int i = 0; i < NUM_OF_NUMERS_TO_TEST; i++) {
		temp_num1 = test_array[i] / 10;
		temp_num2 = test_array[i] % 10;
		temp_num_char1[0] = temp_num1 + '0';
		temp_num_char2[0] = temp_num2 + '0';
		temp_num_str[0] = temp_num_char1[0];
		temp_num_str[1] = temp_num_char2[0];
		temp_num_str[2] = '\r';
		temp_num_str[3] = '\n';
		temp_num_str[4] = '\0';
		ret_val1 = WriteFileWrap(tasks_file, temp_num_str, NUM_OF_CHARS_IN_NUM);
		if (ret_val1 != SUCCESS)
		{
			CloseHandleWrap(tasks_file);
			return ret_val1;
		}
	}
	return ret_val1;
}
//inputs: test_priorities- handle to the tasks file, test_array- array of tasks to preform
//output:ret_val1- none
//functionality : this function generates test_array- array of numbers to preform tesks on, generated randomlly, and an array of priorities.
void generate_array_tests(int test_array[], int test_priorities[]) {
	int i = 0;
	for (i = 0; i < NUM_OF_NUMERS_TO_TEST; i++) {
		test_array[i] = (rand() % 50) + 10;
		test_priorities[i] = i * NUM_OF_CHARS_IN_NUM;
	}
}

// A utility function to swap 2 integers 
void swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

//inputs:arr-array, n- number of items in the array
// functionality :A function to generate a random permutation of arr[] 
void randomize(int arr[], int n)
{
	//this function randomize a seed − This is an integer value to be used as seed by the pseudo-random number generator algorithm
	srand(time(NULL));

	// Start from the last element and swap one by one. We don't 
	// need to run for the first element that's why i > 0 
	for (int i = n - 1; i > 0; i--)
	{
		// Pick a random index from 0 to i 
		int j = rand() % (i + 1);

		// Swap arr[i] with the element at random index 
		swap(&arr[i], &arr[j]);
	}
}
