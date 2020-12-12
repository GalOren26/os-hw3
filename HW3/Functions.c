#include "Functions.h"
#include <assert.h>
#include < stdlib.h >



// ******** valiation of data and parms ************ 

	void CheakArgs(int argc,int excpted_num_of_args)
{
	if (argc < excpted_num_of_args)
	{
		printf("ERROR: Not enough input arguments");
		exit(ERR_CODE_NOT_ENOUGH_ARGUMENTS);
	}
	if (argc > excpted_num_of_args)
	{
		printf("ERROR: Too many input arguments");
		exit(ERR_CODE_TOO_MANY_ARGUMENTS);
	}
}

	int valid_PTR(void* ptr )
	{
		if (!ptr)
		{
			printf(" invalid input pointer parm");
			return INVALID_INPUT_PARM_EMPTY_POINTER;
		}
		return SUCCESS;
	}

	int FreeHandelsArray(HANDLE* handels, int len)
	{
		int ret_val = 0;
		ret_val=valid_PTR(handels);
		if (ret_val != SUCCESS)
		{
			return ret_val;
		}
		for (int i = 0; i < len;i++)
		{
			if (handels[i]!=0)
				CloseHandleWrap(handels[i]);
		}
		if (handels != 0)
			free(handels);
		return SUCCESS;
	}

	int WaitForSingleObjectWrap(HANDLE handle, uli time_ms)
	{
		int wait_code = WaitForSingleObject(handle, time_ms);
		if (wait_code != WAIT_OBJECT_0)
		{
			printf("problem with WaitForSingleObject ,error code is %d \n\n", GetLastError());
			return TIME_OUT_THREAD;
		}
		return SUCCESS;
	}

	int WaitForMultipleObjectsWrap(uli num_of_threads, HANDLE* handle_arr, uli time_ms, BOOL bWaitAll)
	{

		int wait_code = WaitForMultipleObjects(num_of_threads, handle_arr, TRUE, INFINITE);
		if (WAIT_OBJECT_0 != wait_code)
		{
			printf("problem with WaitForMultipleObject ,error code is %d \n\n", GetLastError());
			return TIME_OUT_THREAD;
		}
		return SUCCESS;
	}

	int  CheckAlocation(void* p_arr)
	{
		if (p_arr == NULL) {
			printf_s("MEMORY_ALLOCATION_FAILURE.\n");
			return MEMORY_ALLOCATION_FAILURE;
		}
		return SUCCESS;
	}
	int CheakHandle(HANDLE my_handle)
	{
		if (my_handle == INVALID_HANDLE_VALUE)
		{
			printf_s("INVALID_HANDLE. error code %d", GetLastError());
			return  GetLastError();
		}
		return SUCCESS;
	}

	int CheakIsAnumber(char* str)
	{
		int ret_val = 0;
		ret_val = valid_PTR(str);
		if (ret_val != SUCCESS)
		{
			return ret_val;
		}
		/*This function cheak if string is a number - used for validate the value of key in this code */
		for (int i = 0; str[i] != '\0'; i++)
		{
			if (!isdigit(str[i]))
			{
				return FALSE;
			}
		}
		return TRUE;
	}
//****************File methods**************

	int read_number_of_line_and_end_of_lines(HANDLE file, PDWORD OUT num_of_lines_out, OUT uli** p_end_of_lines_out)
	{
		/*return the  number of line in file  and the places in the file of each  end of line */
		int ret_val = 0;
		ret_val=CheakHandle(file);
		if (ret_val != SUCCESS)
			return ret_val;
		// cheak_file_size_in_order to read file.
		LARGE_INTEGER  len_li;
		if (GetFileSizeEx(file, &len_li) == 0)
		{
			printf("empty file . error code %d", GetLastError());
			return EMPTY_FILE;
		}
		if (len_li.u.HighPart != 0)
		{	//asume file not more than 4gb 
			printf("File is Too big need to be less then 4 GB !");
			return FILE_IS_TOO_BIG;
		}
		DWORD len = len_li.u.LowPart;
		char* my_file_buff = 0;
		my_file_buff = (char*)calloc(len, sizeof(char));
		ret_val=CheckAlocation(my_file_buff);
		if (ret_val != SUCCESS&& my_file_buff==0)
			return ret_val;
		ret_val=ReadFileWrap(len, file, my_file_buff);
		if (ret_val != SUCCESS)
		{
			free(my_file_buff);
			return ret_val;
		}
		DWORD num_of_lines = 0; 
		/* allocatre array to store end of lines, size bounded by the size of file and after fill will be shrink.*/
		uli* p_end_of_lines_temp = calloc( len, sizeof(uli));
		ret_val =CheckAlocation(p_end_of_lines_temp);
		if (ret_val != SUCCESS)
		{
			free(my_file_buff);
			return ret_val;
		}
		uli place = 0; 
		DWORD pos_in_file;
		for (pos_in_file = 0; pos_in_file < len; pos_in_file++)
		{
			if (my_file_buff[pos_in_file] == '\n')
			{
				num_of_lines++;
				p_end_of_lines_temp[place++] = pos_in_file;
			}
		}
		if (my_file_buff[len - 1] != '\n')// cheak if last line is without new line
		{
			num_of_lines++;
			p_end_of_lines_temp[place++] = pos_in_file-1;

		}
		uli* p_end_of_lines = calloc(place, sizeof(uli));
		ret_val=CheckAlocation(p_end_of_lines);
		if (ret_val != SUCCESS)
		{
			free(my_file_buff);
			free(p_end_of_lines_temp);
			return ret_val;
		}
		if (place < len)
			memcpy(p_end_of_lines, p_end_of_lines_temp, sizeof(uli) *place);
		free(p_end_of_lines_temp); 
		free(my_file_buff);
		*num_of_lines_out = num_of_lines;
		*p_end_of_lines_out = p_end_of_lines;
		return SUCCESS;
	}

	int OpenFileWrap( LPCSTR str, DWORD mode,HANDLE * OUT hFile)
	{
		//CreateFileA wrap 
		int ret_val = 0;
		*hFile = CreateFileA(str, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ| FILE_SHARE_WRITE, NULL, mode, FILE_ATTRIBUTE_NORMAL, NULL);
		return CheakHandle(*hFile);
	}
	
	int FreeArray(void** arr, int len)
	{
		int ret_val = 0;
		ret_val = valid_PTR(arr);
		if (ret_val != SUCCESS)
		{
			return ret_val;
		}
		for (int i = 0; i < len;i++)
		{
			if (arr[i]!=NULL)
				free(arr[i]);
		}
		free(arr);
		return SUCCESS;
	}

	int CloseHandleWrap(HANDLE file)
	{
		//CloseHandle wrap 
		BOOL  file_status = CloseHandle(file);
		if (!file_status)
		{
			printf("Failed to close file.error code %d", GetLastError());
			return FAILAD_TO_CLOSE_FILE;
			//not exit couse try best effort to close more files. 
		}
		return SUCCESS;
	}
	int CreateSemphoreWrap(int max_count, HANDLE* OUT semphore,int initialcount)
	{
		*semphore = CreateSemaphoreA(
			NULL,	/* Default security attributes */
			initialcount,		/* Initial Count - all slots are empty */
			max_count,		/* Maximum Count */
			NULL); 

		if (*semphore == NULL)
		{
			printf("problem with create semphore,error code %d", GetLastError());
			return PROBLEM_CREATE_SEMPHORE;
		}
		return SUCCESS;
	}

	int CreateMutexWrap(BOOL bInitialOwner, HANDLE* OUT mutex )
	{

		*mutex = CreateMutexA(
			NULL,	/* Default security attributes */
			bInitialOwner,		/* Set if the creator of the mutex is lock it after it create it.  */
			NULL);

		if (*mutex == NULL)
		{
			printf("problem with create mutex,error code %d", GetLastError());
			return PROBLEM_CREATE_MUTEX;
		}
		return SUCCESS;
	}

	int OpenSemphoreWrap(HANDLE* OUT semphore, const char* name)
	{
		*semphore = OpenSemaphoreA(
			SYNCHRONIZE,
			FALSE,		
			name);

		if (*semphore == NULL)
		{
			printf("problem with OPEN semphore,error code %d", GetLastError());
			return PROBLEM_OPEN_SEMPHORE;
		}
		return SUCCESS;
	}

	int ReleaseSemphoreWrap(HANDLE semphore, int lReleaseCount)
	{
		int wait_code = ReleaseSemaphore(semphore, lReleaseCount, NULL);
		if (wait_code == NULL)
		{
			printf("problem with realease semphore ,error code%d", GetLastError());
			return ERROR_RELEASE_SEMPHORE;
		}
		return SUCCESS;
	}

	int ReleaseMutexeWrap(HANDLE mutex)
	{
		int wait_code = ReleaseMutex(mutex);
		if (wait_code == NULL)
		{
			printf("problem with realease Mutex,error code  %d", GetLastError());
			return ERROR_RELEASE_MUTEX;
		}
		return SUCCESS;
	}


	int FindPrimeComponets(int prime , int* OUT prime_components)
	{
		return 0;
	}

	int sort(int* prime_components)
	{
		return 0;
	}

	int FormatNumberString(int* prime_components, char* OUT str)
	{
		return 0;
	}

	int  ReadFileWrap(DWORD len, HANDLE file,char* my_file_buff )
	{
		//WRAP TO ReadFile 
	
		DWORD lpNumberOfBytesRead = 0;
		if (ReadFile(file, (LPVOID)my_file_buff, len, &lpNumberOfBytesRead, NULL) == 0)
		{
			printf("error read file . error code %d", GetLastError());
			return ERROR_READ_FILE;
		}
		return SUCCESS;
	}

	int SetEndOfFileWarp(LPCSTR output_path, uli end_pos, DWORD mode)
	{
		HANDLE output_file;
		int ret_val = 0;
		int ret_val2 = 0;
		OpenFileWrap(output_path, CREATE_ALWAYS, &output_file);
		ret_val = CheakHandle(output_file);
		//TODO
		if (ret_val != SUCCESS)
		{
			return ret_val;
		}
		// set EOF at the end of the input file 
		ret_val = SetFilePointer(output_file, end_pos,NULL, mode);
		if (ret_val == INVALID_SET_FILE_POINTER)
		{
			printf("problem with set file-pointer %d \n", GetLastError());
			CloseHandleWrap(output_file);
			return ret_val;
		}
		ret_val = SetEndOfFile(output_file);
		if (ret_val==0)
		{
			printf("error with set eof ,error code %d", GetLastError());
		}
		ret_val2 = CloseHandleWrap(output_file);
		if (ret_val == 0 || ret_val2 != SUCCESS)
			return ret_val;
		return SUCCESS;
	}

	int WriteFileWrap(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite)
	{
		DWORD lpNumberOfBytesRead = 0;
		if (WriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, &lpNumberOfBytesRead, NULL) == 0)
		{
			printf("error write file . error code %d", GetLastError());
			return FAILED_WRITE_TO_FILE;
		}
		return SUCCESS;
	}

	int SetFilePointerWrap(HANDLE input_file, uli pos, DWORD mode)
	{
		DWORD retval = SetFilePointer(input_file, pos, NULL, mode);

		if (retval == INVALID_SET_FILE_POINTER)
		{
			printf("INVALID_SET_FILE_POINTER");
			return INVALID_SET_FILE_POINTER;
		}
		return SUCCESS;
	}


	//int setup_memory_menagment(MemoryTracker* OUT MemTracker)
	//{
	//	MemTracker->max_size = START_ALLOCATION_SIZE;
	//	MemTracker->count = 0;
	//	MemTracker->array_tracker = ((MemoryTrackerElement*)calloc(START_ALLOCATION_SIZE, sizeof(MemoryTrackerElement)));
	//	return CheakAlocation(MemTracker->array_tracker);

	//}

	//int push_element_memory_mangment(MemoryTracker* MemTracker, MemoryTracker element)
	//{
	//	int ret_val = 0;
	//	ret_val = valid_PTR(MemTracker);
	//	if (ret_val != SUCCESS)
	//	{
	//		return ret_val;
	//	}
	//	if (MemTracker->count < MemTracker->max_size)
	//	{
	//		MemTracker->max_size *= 2;
	//		MemTracker->array_tracker = (MemoryTracker*)realloc(MemTracker->array_tracker, sizeof(MemoryTracker) * sizeof(MemTracker->max_size));
	//		ret_val = CheakAlocation(MemTracker->array_tracker);
	//		if (ret_val != SUCCESS)
	//		{
	//			return ret_val;
	//		}
	//	}
	//	MemTracker->array_tracker[MemTracker->count] = element;
	//	MemTracker->count++;
	//}

	//void FreeAll(MemoryTracker* MemTracker)
	//{
	//	int ret_val = 0;
	//	ret_val = valid_PTR(MemTracker);
	//	if (ret_val != SUCCESS)
	//	{
	//		return ret_val;
	//	}
	//	for (int i = 0; i < MemTracker->count; i++)
	//	{
	//		if (MemTracker->array_tracker[i].array_handle == NULL)
	//		{
	//			continue;
	//		}
	//		if (MemTracker->array_tracker[i].type == ARRAY)
	//		{
	//			if (MemTracker->array_tracker[i].count == 1)
	//				free(MemTracker->array_tracker[i].array_handle);
	//			else
	//			{
	//				for (int j = 0; j < MemTracker->array_tracker[i].count; j++)
	//					free(MemTracker->array_tracker[i].array_handle[i]);
	//			}
	//		}

	//	}
	//}