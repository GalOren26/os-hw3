#include "Functions.h"
#include "ThreadFuncs.h"

#include <assert.h>
#include < stdlib.h >

DWORD WINAPI read(LPVOID lpParam)
{
	int ret_val1 = 0;
	parssing_data* params;
	HANDLE InputHandle;
	params = (parssing_data*)lpParam;
	HANDLE input_file;
	printf("Hello from thread num %d, I am reader \n", GetCurrentThreadId());


	/// ---------TO-DO ---------
	/// allocate array_poistions of size of number lines --ret_val1=cheakaloocation () if ! success return ret_val1 ; free when not neede 
	/// -for i in range(params->number_of_lines):
	///			pop ( params->fifo) 
	/// openfilewarp (params->input_path); cheak forfailure 
	/// allocate array numbers  of numbers(int) size  of lines ; 
	/// 
	///	-for i in range(params->number_of_lines): 
	///		setfilepointerwrap(arr[i]);
	///		char* line;  
	///		lock_read(params_lock)
	///		readline(&line) 
	/// 	realse_read(params_lock);
	///		cheakisnumber(line) 
	///		numbers[i]= atoi(line) 
	///		free (array_poistions) ; 
	/// 
	/// array of char *  string of strings 
	///	for i in range(params->number_of_lines):  
	///		FindPrimeComponets ()
	///		formatstringpeime() 
	///		strings.append(string)  
	///	
	/// 	----expend file ---
	/// var=  setfilepointer (end ) -parm that tak the sdistance from start 
	/// set file pointer (end , size (all the strigns )
	/// for i in range(params->number_of_lines):  
	///		lock_write () 
	///		writefilewrap
	///		relasewrite 
	/// 
	/// 
	/// 
	///  
	///		
	/// 
	/// 
	/// 
	/// 
	/// 
	/// 
	/// 
	/// 
	/// 

	//if (params->lock->readers == 1)
	//{
	//	release_read(params->lock);
	//	if (params->lock->readers == 0)
	//		lock_write(params->lock);
	//}
	//read_lock(params->lock);
	//ret_val1=OpenFileWrap(params->input_path, OPEN_EXISTING, &input_file);
	//if (ret_val1 != SUCCESS)
	//{
	//	return ret_val1;
	//}
	//char my_file_buff[100] = { 0 }; 
	//DWORD num_of_bytes_read;
	//ReadFileWrap(params->end_pos, input_file, my_file_buff,&num_of_bytes_read);
	//printf("%s\n\n", my_file_buff);
	return SUCCESS;
}


int Createmultiplethreads(parssing_data* p_params,uli num_of_threads)
{
	int ret_val = 0;
	int ret_val2 = 0;
	int num_of_lines = p_params->number_of_lines;
	HANDLE* p_thread_handles = (HANDLE * )calloc(num_of_threads, sizeof(HANDLE)); // each cell in the array, contains params for thread
	ret_val=CheckAlocation(p_thread_handles);
	if (ret_val != SUCCESS)
	{
		goto free4;
	}
	DWORD* p_thread_ids = (DWORD * )calloc(num_of_threads, sizeof(DWORD)); // each cell in the array, contains params for thread
	CheckAlocation(p_thread_ids);
	if (ret_val != SUCCESS)
	{
		goto free3;
	}
	parssing_data** thread_params = (parssing_data**)calloc(num_of_threads, sizeof(parssing_data*));// array to contain: functions params for each thread
	CheckAlocation(thread_params);
	if (ret_val != SUCCESS)
	{
		goto free2;
	}
	int num_threads_to_add_more_line = num_of_lines % num_of_threads;
	uli lines_per_thread = (num_of_lines / num_of_threads);
	short add_one_more_line = 1;
	for (int i = 0; i < num_of_threads; i++)
	{
		if (i == num_threads_to_add_more_line)
			add_one_more_line = 0;
		thread_params[i] = (parssing_data*)calloc(1, sizeof(parssing_data));
		ret_val = CheckAlocation(thread_params[i]);
		if (ret_val != SUCCESS)
			goto free1;
		thread_params[i]->fifo = p_params->fifo;
		thread_params[i]->input_path = p_params->input_path;
		thread_params[i]->lock= p_params->lock;
		thread_params[i]->number_of_lines = lines_per_thread + add_one_more_line;
		ret_val = CreateThreadSimple(read, (LPVOID)thread_params[i], &p_thread_ids[i], &p_thread_handles[i]);
		if (ret_val != SUCCESS)
			goto free1;
	}
	//WAIT THERS WILL FINISH 
	ret_val = WaitForMultipleObjectsWrap(num_of_threads, p_thread_handles, TIMEOUT_IN_MILLISECONDS, TRUE);
	//FREE
free1: 
	FreeHandelsArray(p_thread_handles, num_of_threads);
	p_thread_handles = 0;
	FreeArray(thread_params, num_of_threads);
	thread_params = 0;
free2:	free(p_thread_ids);
free3:	if (p_thread_handles!=0)
		free(p_thread_handles);
free4:	return ret_val;
}

 int CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
		LPVOID p_thread_parameters,
		LPDWORD p_thread_id, HANDLE* OUT thread_handle)
	{

		if (NULL == p_start_routine)
		{
			printf("Error when creating a thread");
			printf("Received null pointer");
			return INVALID_INPUT_PARM_EMPTY_POINTER;
		}

		if (NULL == p_thread_id)
		{
			printf("Error when creating a thread");
			printf("Received null pointer");
			return INVALID_INPUT_PARM_EMPTY_POINTER;
		}

		*thread_handle = CreateThread(
			NULL,                /*  default security attributes */
			0,                   /*  use default stack size */
			p_start_routine,     /*  thread function */
			p_thread_parameters, /*  argument to thread function */
			0,                   /*  use default creation flags */
			p_thread_id);        /*  returns the thread identifier */
		if (thread_handle == NULL)
		{
			printf("failed create thread %d", GetLastError());
			return GetLastError();
		}
		return SUCCESS;
	}

//------- prime number functions--------------

 int FindPrimeComponets(int prime, int* OUT prime_components)
 {
	 //int n = 999999999;
	 //int* arr[30] = { 0 };
	 int index = 0;
	 while (prime % 2 == 0) {
		 prime_components[index] = 2;
		 index += 1;
		 prime = prime / 2;
	 }
	 for (int i = 3; i * i <= prime; i += 2) {
		 while (prime % i == 0) {
			 prime_components[index] = i;
			 index += 1;
			 prime = prime / i;
		 }
	 }
	 if (prime > 2) {
		 prime_components[index] = prime;
	 }
	 return index;
 }
 void FormatNumberString(int* prime_components, char* OUT prime_factors_by_format, int number_of_components)
 {
	 int i = 0;
	 for (int i = 0; i < number_of_components - 1; i++) {
		 prime_factors_by_format[i] = prime_components[i] + '0';
		 prime_factors_by_format[i + 1] = ',';
		 i++;
	 }
	 prime_factors_by_format[i] = prime_components[i] + '0';
	 prime_factors_by_format[i + 1] = '\0';
 }

 void printByFormat(int number, char* prime_factors_by_format) {
	 printf("The prime factors of %d are: %s\r\n", number, prime_factors_by_format);
 }
