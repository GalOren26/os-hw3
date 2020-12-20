#include "Functions.h"
#include <assert.h>
#include < stdlib.h >



// ******** valiation of data and parms ************ 
	//input : input_file- argc- the number of arguments we got threw command line , expected_num_of_args- the number of arguments we expact to have
	//output:number-none
	//fuctionality : this function checks if we got the number of arguments we expacted

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
	//input : ptr- a pointer
	//output:number-indicates whether the function succeeded or not
	//fuctionality : this function checks it a pointer is valid or not

	int valid_PTR(void* ptr )
	{
		if (!ptr)
		{
			printf(" invalid input pointer parm");
			return INVALID_INPUT_PARM_EMPTY_POINTER;
		}
		return SUCCESS;
	}
	//input : handles - an array of handles. len - number of handles
	//output:number-indicates whether the function succeeded or not
	//fuctionality : this function closes all the handles in a file

	int FreeHandelsArray(HANDLE* handels, int len)
	{
		int ret_val = 0;
		ret_val=valid_PTR(handels);
		if (ret_val != SUCCESS)
		{
			return ret_val;
		}
for (int i = 0; i < len; i++)
{
	if (handels[i] != 0)
		CloseHandleWrap(handels[i]);
}
if (handels != 0)
free(handels);
return SUCCESS;
	}
	//input :handle of a thread to wait for ,time_ms- time limitation to wait
	//output:number-indicates whether the function succeeded or not
	//fuctionality : this function wraps the windows-API function "waitForSingleObject" and checks if it succeeded or not

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
	//input :num_of_threads- the number of threads  ,handle_arr- an array of handles to threads,time_ms- time limitation to wait
	//output:number-indicates whether the function succeeded or not
	//fuctionality : this function wraps the windows-API function "waitFormultypleObjects" and checks if it succeeded or not
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
	//input :p_arr- a pointer 
	//output:number-indicates whether the function succeeded or not
	//fuctionality : this function checks if memory allocation succeeded
	int  CheckAlocation(void* p_arr)
	{
		if (p_arr == NULL) {
			printf_s("MEMORY_ALLOCATION_FAILURE.\n");
			return MEMORY_ALLOCATION_FAILURE;
		}
		return SUCCESS;
	}
	//input :my_handle - a handle
	//output:number-indicates whether the function succeeded or not
	//fuctionality : this function checks if a handle is valid
	int CheakHandle(HANDLE my_handle)
	{
		if (my_handle == INVALID_HANDLE_VALUE)
		{
			printf_s("INVALID_HANDLE. error code %d\n", GetLastError());
			return  GetLastError();
		}
		return SUCCESS;
	}
	//input :str- a string form of a number
	//output:number-indicates whether the function succeeded or not
	//fuctionality : this function checks whether a string is a number or not
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
		return SUCCESS;
	}
	//****************File methods**************
	//input :input_file- to read from, line - a buffer to read the line in to.
	//output:number-indicates whether the function succeeded or not
	//fuctionality : this function reads a line in to a buffer
	int ReadLine(HANDLE input_file, char** OUT line)
	{
		BOOL eof_found = FALSE;
		uli expand_factor = 1;
		uli curser_index = 0;
		int ret_val = 0;
		int start_pos;
		ret_val = SetFilePointerWrap(input_file, 0, FILE_CURRENT, &start_pos);
		if (ret_val != SUCCESS)
		{
			return ret_val;
		}
		char* line_temp = calloc(NUM_OF_BYTES_TO_READ+1, sizeof(char));
		ret_val = CheckAlocation(line_temp);
		if (ret_val != SUCCESS)
		{
			return ret_val;
		}
		while (!eof_found)
		{
			DWORD num_of_bytes_read;
			ret_val = ReadFileWrap(NUM_OF_BYTES_TO_READ, input_file, &line_temp[curser_index], &num_of_bytes_read);
			if (ret_val != SUCCESS)
				return ret_val;
			if (num_of_bytes_read < NUM_OF_BYTES_TO_READ)
				eof_found = TRUE;
			for (; line_temp[curser_index] != 0 && line_temp[curser_index] != '\n'; curser_index++);
			if (line_temp[curser_index] == '\n')
			{
				ret_val = SetFilePointerWrap(input_file, start_pos+curser_index+1, FILE_BEGIN,NULL);
				if (ret_val != SUCCESS)
				{
					return ret_val;
				}

				line_temp[curser_index - 1] = '\0';
				*line = (char*)calloc(curser_index , sizeof(char));
				ret_val = CheckAlocation(line_temp);
				if (ret_val != SUCCESS)
				{
					free(line_temp);
					return ret_val;
				}
				memcpy(*line, line_temp, curser_index);
				free(line_temp);
				return SUCCESS;
			}
			expand_factor++;
			char* line_temp_alloc=(char*)realloc(line_temp, expand_factor * NUM_OF_BYTES_TO_READ+1);
			ret_val = CheckAlocation(line_temp_alloc);
			if (ret_val != SUCCESS)
			{
				free(line_temp);
				return ret_val;
			}

			else {
				line_temp = line_temp_alloc;
				memset(line_temp + curser_index, 0, NUM_OF_BYTES_TO_READ + 1);
			}
		}

		return NO_NEW_LINE;
	}
	//input :mode - purpose we want to use the file,
	//output:number-indicates whether the function succeeded or not
	//fuctionality : this function wraps the windows API function createFileA and checks if it succeeded
	int OpenFileWrap( LPCSTR str, DWORD mode,HANDLE * OUT hFile)
	{ 
		int ret_val = 0;
		*hFile = CreateFileA(str, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ| FILE_SHARE_WRITE, NULL, mode, FILE_ATTRIBUTE_NORMAL, NULL);
		return CheakHandle(*hFile);
	}
	
	int CloseHandleWrap(HANDLE file)
	{
		BOOL  file_status = CloseHandle(file);
		if (!file_status)
		{
			printf("Failed to close file.error code %d", GetLastError());
			return FAILAD_TO_CLOSE_FILE;
			//not exit couse try best effort to close more files. 
		}
		return SUCCESS;
	}
	int FreeArray(void** arr, int len)
	{
		int ret_val = 0;
		ret_val = valid_PTR(arr);
		if (ret_val != SUCCESS)
		{
			return ret_val;
		}
		for (int i = 0; i < len; i++)
		{
			if (arr[i] != NULL)
				free(arr[i]);
		}
		free(arr);
		return SUCCESS;
	}
	//input :max_count - for the semaphore object, semaphore - handle to semaphore, initialcount - number to start count from in the semaphore
	//output:number-indicates whether the function succeeded or not
	//fuctionality : this function wraps the windows API createSemaphoreA function
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
	//input :initialOwner- sets if it's lock after creation or not, mutex- handle to a mutex
	//output:number-indicates whether the function succeeded or not
	//fuctionality : this function wraps the windows API createMutexA function
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
	//input  semaphore - handle to semaphore, name- unique identifier to the semaphore
	//output:number-indicates whether the function succeeded or not
	//fuctionality : this function wraps the windows API openSemaphoreA function,and allows to open a semaphore
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
	//input  semaphore - handle to semaphore, name- unique identifier to the semaphore
	//output:number-indicates whether the function succeeded or not
	//fuctionality : this function wraps the windows API releasesemaphoreA function,and allows to release a semaphore
	int ReleaseSemphoreWrap(HANDLE semphore, int lReleaseCount)
	{
		int wait_code = ReleaseSemaphore(semphore, lReleaseCount, NULL);
		if (wait_code == 0)
		{
			printf("problem with realease semphore ,error code%d", GetLastError());
			return ERROR_RELEASE_SEMPHORE;
		}
		return SUCCESS;
	};
	//input:  mutex - handle to semaphore, name- unique identifier to the semaphore
	//output:number-indicates whether the function succeeded or not
	//fuctionality : this function wraps the windows API releasesemaphoreA function,and allows to release a semaphore
	int ReleaseMutexeWrap(HANDLE mutex)
	{
		int wait_code = ReleaseMutex(mutex);
		if (wait_code == 0)
		{
			printf("problem with realease Mutex,error code  %d", GetLastError());
			return ERROR_RELEASE_MUTEX;
		}
		return SUCCESS;
	}
	//input: len-number of bytes to read, file- handle to the file, my_file_buff-a buffer to read the file in to. NumerOfBytesRead-the number of bytes was read
	//output:number-indicates whether the function succeeded or not
	//fuctionality : this function wraps the windows API ReadFile function
	int  ReadFileWrap(DWORD len, HANDLE file,char* my_file_buff, DWORD* NumberOfBytesRead )
	{
		//WRAP TO ReadFile 
		if (ReadFile(file, (LPVOID)my_file_buff, len, NumberOfBytesRead, NULL) == 0)
		{
			printf("error read file . error code %d", GetLastError());
			return ERROR_READ_FILE;
		}
		return SUCCESS;
	}

	//input: input_file- the file we want to move it's pointer, offst_len-distance to move
	//output:number-indicates whether the function succeeded or not
	//fuctionality : this function wraps the windows API SetEndOfFile function 
	int SetEndOfFileWarp(HANDLE  input_file, uli offset_len,int mode )
	{
		int ret_val = 0;
		uli position_in_file = 0;
		// set EOF at the end of the input file 
		ret_val = SetFilePointer(input_file, offset_len,&position_in_file, mode);
		if (ret_val == INVALID_SET_FILE_POINTER)
		{
			printf("problem with set file-pointer %d \n", GetLastError());
			return ret_val;
		}
		ret_val = SetEndOfFile(input_file);
		if (ret_val==0)
		{
			printf("error with set eof ,error code %d", GetLastError());
			return ret_val;
		}
		return SUCCESS;
	}
	//input: hfile- handle to the file we wan to write to, ipbuffer- we write from it to the file, nNumberOfBytesToWrite- maximal number of bytes we want to write
	//output:number-indicates whether the function succeeded or not
	//fuctionality : this function wraps the windows API WriteFile function 
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


	//-------------------strings 
	//input : num- the number we want to convert to it's string form, str-the string number will be kept in this variable
	//output:number-indicates whether the function succeeded or not
	//fuctionality : this function get's the string form of a int number

	int  convert_int_to_str(int num, char** OUT str)
	{
		int ret_val = 0;
		int counter = find_len_number(num);
		char* my_str = (char*)calloc(counter + 1, sizeof(char));
		ret_val = CheckAlocation(my_str);
		if (ret_val != SUCCESS)
		{
			return ret_val;
		}
		my_str[counter] = '\0';
		for (int i = 1;i <= counter; i++)
		{
			my_str[counter - i] = num % 10 + '0';
			num /= 10;
		}
		*str = my_str;
		return SUCCESS;
	}
	//input : num- number to find it's length
	//output: num's length
	//fuctionality : this function returns the number of digit's in "num|
	int find_len_number(int num)
	{
		int counter = 0;
		while (num != 0)
		{
			num /= 10;
			counter++;
		}
		return counter;
	}

	//input : input_file- the file we want to move it's ponter's position, DistanceToMove - number of chars to move,  FromWhereToMove- the start char to move from,
	//output:number-indicates whether the function succeeded or not
	//fuctionality : this function moves the file pointer to the designated location

	int SetFilePointerWrap(HANDLE input_file, uli DistanceToMove, DWORD FromWhereToMove, DWORD* OUT PositionAfterSet)
	{
		DWORD retval;
		if (PositionAfterSet != NULL)
		{
			retval = SetFilePointer(input_file, DistanceToMove, NULL, FromWhereToMove);
			*PositionAfterSet = retval;
		}
		else 
			retval = SetFilePointer(input_file, DistanceToMove, NULL, FromWhereToMove);
		if (retval == INVALID_SET_FILE_POINTER)
		{
			printf("INVALID_SET_FILE_POINTER");
			return INVALID_SET_FILE_POINTER;
		}
		return SUCCESS;
	}
