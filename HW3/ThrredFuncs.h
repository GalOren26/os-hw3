#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "HardCodedData.h"
#include <Windows.h>
typedef struct
{
	uli start_pos;
	uli end_pos;
	LPCSTR  output_path;
	LPCSTR input_path;
	uli key;
	int operation;
	HANDLE* main_sem;
	HANDLE* thread_sem;
}parssing_data;



int Createmultiplethreads(int num_of_threads, uli num_of_lines, uli* end_of_lines, parssing_data* params);

//took from the Recritation, the wrap of create threads function.
static int CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPVOID p_thread_parameters,
	LPDWORD p_thread_id, HANDLE* OUT thread_handle);