
#include "../HW3/Functions.h"
#include "../HW3/HardCodedData.h"
#include <time.h>
#include "test_functions.h"


int main(int argc, char* argv)
{
	int temp_num1, temp_num2, ret_val1, argc_test = 5;
	char* argv_test[] = { "TasksTest.txt", "TasksPrioritiesTest.txt","6", "6" }; //input for the tests
	char* TasksPrioritiesTest = "TasksPrioritiesTest.txt";
	char* file_name = "TasksTest.txt";
	HANDLE tasks_file;
	HANDLE priorities_file;
	int test_array[NUM_OF_NUMERS_TO_TEST] = { 0 };
	int test_priorities[NUM_OF_NUMERS_TO_TEST] = { 0 };
	generate_array_tests(test_array, test_priorities);
	randomize(test_array, NUM_OF_NUMERS_TO_TEST);
	ret_val1 = OpenFileWrap(file_name, OPEN_ALWAYS, &tasks_file);
	if (ret_val1 != SUCCESS)
	{
		CloseHandleWrap(tasks_file);
		return ret_val1;
	}
	ret_val1 = write_to_tasks_file(tasks_file, test_array);
	if (ret_val1 != SUCCESS)
	{
		return ret_val1;
	}

	ret_val1 = OpenFileWrap(TasksPrioritiesTest, OPEN_ALWAYS, &priorities_file);
	if (ret_val1 != SUCCESS)
	{
		CloseHandleWrap(tasks_file);
		return ret_val1;
	}

	ret_val1 = write_to_priorities_file(priorities_file, test_priorities);
	if (ret_val1 != SUCCESS)
	{
		CloseHandleWrap(tasks_file);
		return ret_val1;
	}
	CloseHandleWrap(tasks_file);
	CloseHandleWrap(priorities_file);
}