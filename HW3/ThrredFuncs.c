#include "Functions.h"
#include "ThrredFuncs.h"

#include <assert.h>
#include < stdlib.h >

//****************encryption decryption  methods**************

int Dec_Enc_Block(parssing_data* p_params)
{
	/* the function  Decrypt file by positiong in the file and parms -
	used to implement stage 1 and help for  the threads later.*/
	if (p_params->end_pos == p_params->start_pos)
	{
		return SUCCESS;
	}
		HANDLE input_file;
		HANDLE output_file;
	 int ret_val = OpenFileWrap(p_params->input_path, OPEN_EXISTING, &input_file);
	 //TODO
	 if (ret_val != SUCCESS)
		goto free1;
	 ret_val=SetFilePointerWrap(input_file, p_params->start_pos, FILE_BEGIN);
	 if (ret_val != SUCCESS)
		 goto free2;
	 DWORD len = p_params->end_pos- p_params->start_pos+1;
	 char* my_file_buff = (char* )calloc(len, sizeof(char));
	 ret_val=CheckAlocation(my_file_buff);
	 if (ret_val != SUCCESS)
		 goto free2;
	  ret_val = ReadFileWrap(len,input_file, my_file_buff);
	 if (ret_val == ERROR_READ_FILE)
	 {
		 goto free3;
	 }
	 if (p_params->operation == ENCRYPT)
		 Encryption(my_file_buff, len, p_params->key);
	 else if (p_params->operation == DECRYPT)
		 Decryption(my_file_buff, len, p_params->key);
	 else
	 {
		 ret_val = NOT_VALID_OPREATION;
		 printf("error with flag opreation ");
		 goto free3;
	 }
	// write to file 
	 ret_val = OpenFileWrap(p_params->output_path, OPEN_EXISTING,&output_file);
	 if (ret_val != SUCCESS)
	 {
		 printf("problem with open destmation file ,error code %d", GetLastError());
		 goto free3;
	 }
	 ret_val=SetFilePointerWrap(output_file, p_params->start_pos, FILE_BEGIN);
	 if (ret_val != SUCCESS)
	 {
		 goto free4;
	 }
	 ret_val= WriteFileWrap(output_file, (LPVOID)my_file_buff, len);
	 if (ret_val != SUCCESS)
	 {
		 goto free4;
	 }
	 else
		 ret_val=SUCCESS; 

 free4:
	 CloseHandleWrap(output_file);
 free3:
	 if(my_file_buff!=0)
		 free(my_file_buff);
 free2:
	 CloseHandleWrap(input_file);
 free1:
	 return ret_val;
}


int Decryption(char* coded,uli end_of_block, uli key)
{
	// function for Decryption and enctyption 
	int ret_val = 0;
	ret_val = valid_PTR(coded);
	if (ret_val != SUCCESS)
	{
		return ret_val;
	}
	char ch;
		for (uli i=0 ; i<= end_of_block; i++) { // run over all the string
			ch = coded[i];
			if (ch >= 'a' && ch <= 'z')
			{ 
				coded[i]='a'+moudlo(ch - 'a' - key, 26);
			}
			if (ch >= 'A' && ch <= 'Z')
			{ 
				coded[i] = 'A' +moudlo(ch - 'A' - key, 26);
			}
			if (ch >= '0' && ch <= '9')
			{ 
				coded[i] = '0' + moudlo(ch - '0' - key, 10);
			}
		}
		return SUCCESS;
}
int Encryption(char* coded, uli end_of_block, uli key)
{
	// function for Decryption and enctyption 
	int ret_val = 0;
	ret_val = valid_PTR(coded);
	if (ret_val != SUCCESS)
	{
		return ret_val;
	}
	char ch;
	for (uli i = 0; i <= end_of_block; i++) { // run over all the string
		ch = coded[i];
		if (ch >= 'a' && ch <= 'z')
		{
			coded[i] = 'a' + moudlo(ch - 'a' + key, 26);
		}
		if (ch >= 'A' && ch <= 'Z')
		{
			coded[i] = 'A' + moudlo(ch - 'A' + key, 26);
		}
		if (ch >= '0' && ch <= '9')
		{
			coded[i] = '0' + moudlo(ch - '0' + key, 10);
		}
	}
	return SUCCESS;
}

DWORD WINAPI DecThread(LPVOID lpParam)
{
	int ret_val1 = 0;
	int ret_val2 = 0;
	int wait_code = 0;
	parssing_data* params;
	DWORD wait_result=0;
	params = (parssing_data*)lpParam;
	printf("Hello from thread num %d, I am blocked ,waiting for realese \n", GetCurrentThreadId());
	//OpenSemphoreWrap(&main_wait, p_main_wait_name);
	//OpenSemphoreWrap(&thread_wait, p_thread_wait_name);
	//if (ret_val1 != SUCCESS || ret_val2 != SUCCESS)
	//	return PROBLEM_OPEN_SEMPHORE;
	wait_code = ReleaseSemaphore(params->main_sem,1, NULL);
	//wait_code = ReleaseSemaphore(*p_params->semphore, num_of_threads, &num);
	if (wait_code == 0)
	{
		printf("Error with Release semphore, error code is %d \n", GetLastError());
		return ERROR_RELEASE_SEMPHORE;
	}

	wait_result = WaitForSingleObject(params->thread_sem, INFINITE);
	if (wait_result != WAIT_OBJECT_0)
	{
		printf("problem with WaitForSingleObject ,error code is %d \n\n", GetLastError());
		return TIME_OUT_THREAD;
	}
	printf("Hello from thread num %d, I am free and start to run,yayy :)) \n", GetCurrentThreadId());
	int ret_val = Dec_Enc_Block(lpParam);
	assert(ret_val == SUCCESS);
	return  SUCCESS; 
}

int Createmultiplethreads(volatile int num_of_threads, uli num_of_lines, uli* end_of_lines, parssing_data* p_params)
{
	int ret_val = 0;
	int ret_val2 = 0;
	/*create params to thread */
	HANDLE* p_thread_handles = (HANDLE * )calloc(num_of_threads, sizeof(HANDLE)); // each cell in the array, contains params for thread
	ret_val=CheckAlocation(p_thread_handles);
	if (ret_val != SUCCESS)
	{
		goto free1;
	}
	DWORD* p_thread_ids = (DWORD * )calloc(num_of_threads, sizeof(DWORD)); // each cell in the array, contains params for thread
	CheckAlocation(p_thread_ids);
	if (ret_val != SUCCESS)
	{
		goto free2;
	}
	parssing_data** thread_params = (parssing_data**)calloc(num_of_threads, sizeof(parssing_data*));// array to contain: functions params for each thread
	CheckAlocation(thread_params);
	if (ret_val != SUCCESS)
	{
		goto free3;
	}
	/*SetEndOfFileWrap(LPCSTR output_path,)*/
    ret_val= SetEndOfFileWarp(p_params->output_path, p_params->end_pos, FILE_BEGIN);
	if (ret_val != SUCCESS)
	{
		goto free4;
	}
	int num_threads_to_add_more_line = num_of_lines % num_of_threads;
	uli lines_per_thread = (num_of_lines / num_of_threads);
	short add_one_more_line = 1;
	uli current_line_place = -1;//couse we start count from 0 the indexs 
	//crate and init params to therad in for loop
	for (int i = 0; i < num_of_threads; i++)
	{
		if ( i ==num_threads_to_add_more_line )
		{
			add_one_more_line = 0;
		}
		// params need to be allocate dynamiclly->init params 
		thread_params[i] = (parssing_data*)calloc(num_of_threads, sizeof(parssing_data)); // each cell in the array, contains params for thread
		ret_val=CheckAlocation(thread_params[i]);
		if (ret_val != SUCCESS)
			goto free4;

		if (i == 0)
		{thread_params[i]->start_pos = 0;}
		else	
			thread_params[i]->start_pos = end_of_lines[current_line_place] + 1;//one after the privous new line  

		current_line_place += add_one_more_line + lines_per_thread;
		thread_params[i]->end_pos = end_of_lines[current_line_place];
		thread_params[i]->output_path = p_params->output_path;
		thread_params[i]->input_path = p_params->input_path;
		thread_params[i]->key = p_params->key;
		thread_params[i]->operation  = p_params->operation;
		thread_params[i]->main_sem = p_params->main_sem;
		thread_params[i]->thread_sem = p_params->thread_sem;
		ret_val= CreateThreadSimple(DecThread, (LPVOID) thread_params[i], &p_thread_ids[i], &p_thread_handles[i]);
		if (ret_val!=SUCCESS)
			goto free4;
	}
	//block main unil all therads are created!!
	for (int i = 0; i < num_of_threads; i++)
	{
		ret_val = WaitForSingleObjectWrap(p_params->main_sem, INFINITE);
		if(ret_val !=SUCCESS)
			goto free4;
	}
	ret_val = ReleaseSemaphore(p_params->thread_sem,num_of_threads,NULL);
	if (ret_val == 0)
	{
		ret_val = TIME_OUT_THREAD;
		printf("Error with Release semphore, error code is %d \n",GetLastError());
		goto free4;
	}
	ret_val=CloseHandleWrap(p_params->thread_sem);
	ret_val2=CloseHandleWrap(p_params->main_sem);
	if (ret_val != SUCCESS || ret_val2 != SUCCESS)
	{
		goto free4;
	}
	ret_val = WaitForMultipleObjects(num_of_threads, p_thread_handles, 1, TIMEOUT_IN_MILLISECONDS);

	if (WAIT_OBJECT_0 != ret_val)
	{
		printf("Error when waiting for threads,Exiting program");
		ret_val = TIME_OUT_THREAD;
	}
	else
		ret_val = SUCCESS;

free4:
	FreeHandelsArray(p_thread_handles, num_of_threads);
	p_thread_handles = 0;
	FreeArray(thread_params, num_of_threads);
	thread_params = 0;
free3:
	if (p_thread_ids != 0)
		free(p_thread_ids);
free2:
	if (p_thread_handles != 0)
		free(p_thread_handles);
free1:
	return ret_val;
}
static int CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
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

