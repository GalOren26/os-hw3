
#include "HardCodedData.h"
#include "Functions.h"
#include "ThreadFuncs.h"
#include "Lock.h"

/*

Authors – Gal Oren -206232506
Rami Ayoub - 315478966
Project – parllel encrypt and dycrypt  kaiser encrpytion with with syncronization of threads start togther \

*/
int main(int argc, char* argv[])
{
	CheakArgs(argc,5);
	DWORD mode = OPEN_EXISTING;
	int ret_val1; 
	LPCSTR input_path = argv[1];
	int num_of_threads = 2;
	Lock *my_lock= InitializeLock(2);
	parssing_data** thread_params = (parssing_data**)calloc(num_of_threads, sizeof(parssing_data*));
	thread_params[0] = (parssing_data*)calloc(num_of_threads, sizeof(parssing_data));
	thread_params[1] = (parssing_data*)calloc(num_of_threads, sizeof(parssing_data));
	thread_params[0]->end_pos=10;
	thread_params[0]->start_pos = 0;
	thread_params[0]->lock= my_lock;
	thread_params[0]->output_path= input_path;
	thread_params[0]->input_path= input_path;
	thread_params[1] = thread_params[0];
	Createmultiplethreads_test(thread_params, num_of_threads);
	
	free(thread_params);
	/* test
	1. multiple read 
	2.try to write 
	3.free read 
	4.try to write 
	5.try to read 
	6.try to write from diffrent thread 
	*/

}




//HANDLE main_wait;
//char* dest_path = 0;
//HANDLE thread_wait;
//int ret_val1 = 0;
//int ret_val2 = 0;
//HANDLE InputHandle;
//int operation;
//// TO-DO chaek if  input is valid path 
//LPCSTR input_path = argv[1];
//ret_val1 = CheakIsAnumber(argv[2]);
//ret_val2 = CheakIsAnumber(argv[3]);
//if (ret_val1 == FALSE || ret_val2 == FALSE)
//{
//	printf("key or num of threads  is not a number  :(");
//	return NOT_A_NUMBER;
//}
//ret_val1 = CheckOperation(argv[4], &operation);
//if (ret_val1 != SUCCESS)
//{
//	printf("flag is neither -e nor -d please enter valid flag ");
//	return ret_val1;
//}
//uli key = atoi(argv[2]);
//int num_of_threads = atoi(argv[3]);
//if (num_of_threads <= 0 || num_of_threads > MAXIMUM_WAIT_OBJECTS)
//{
//	printf("Invalid number of Threads");
//	return(NOT_VALID_INPUT);
//}
//DWORD mode = OPEN_EXISTING;
//ret_val1 = OpenFileWrap(input_path, mode, &InputHandle);
//if (ret_val1 != SUCCESS)
//{
//	return ret_val1;
//}


//DWORD num_of_lines = 0;
//uli* end_of_lines = 0;
//ret_val1 = read_number_of_line_and_end_of_lines(InputHandle, &num_of_lines, &end_of_lines);
////close file anyway
//ret_val2 = CloseHandleWrap(InputHandle);
//if (ret_val2 != SUCCESS)
//{
//	free(end_of_lines);
//	return ret_val2;
//}
//if (ret_val1 != SUCCESS)
//{
//	free(end_of_lines);
//	return ret_val1;
//}
////return output path 
//
//ret_val1 = find_dest_path(input_path, &dest_path, operation);
//if (ret_val1 != SUCCESS)
//{
//	free(end_of_lines);
//	free(dest_path);
//	return ret_val1;
//}
//ret_val1 = CreateSemphoreWrap(num_of_threads, &main_wait);
//ret_val2 = CreateSemphoreWrap(num_of_threads, &thread_wait);
//if (ret_val1 != SUCCESS || ret_val2 != SUCCESS)
//{
//	free(end_of_lines);
//	free(dest_path);
//	return ret_val1;
//}
//parssing_data params = {
//	0,
//	end_of_lines[num_of_lines - 1],
//	dest_path,
//	input_path,
//	key ,
//	operation,
//	main_wait,
//	thread_wait
//};
////Createmultiplethreads(num_of_threads, num_of_lines, end_of_lines, &params);
//free(dest_path);
//free(end_of_lines);
//return SUCCESS;