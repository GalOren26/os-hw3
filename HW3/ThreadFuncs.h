#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "HardCodedData.h"
#include <Windows.h>
#include "Lock.h"
typedef struct
{
	uli start_pos;
	uli end_pos;
	LPCSTR  output_path;
	LPCSTR input_path;
	Lock* lock;
}parssing_data;


//
//int Createmultiplethreads(int num_of_threads, uli num_of_lines, uli* end_of_lines, parssing_data* params);

//took from the Recritation, the wrap of create threads function.
DWORD WINAPI read(LPVOID lpParam);
 int CreateThreadSimple(LPTHREAD_START_ROUTINE p_start_routine,
	LPVOID p_thread_parameters,
	LPDWORD p_thread_id, HANDLE* OUT thread_handle);
 int Createmultiplethreads_test(parssing_data** p_params,int num_of_threads);
 //inputs:prime - the number that we want to get it's prime factors
//	prime_component is an int* array with size 30-
//(max prime factor is 30  couse  num <2^30)
 int FindPrimeComponets(int prime, int* OUT prime_components);
 //format thr string that print the prime factors. 
 void FormatNumberString(int* prime_components, char* OUT prime_factors_by_format, int number_of_components);
//funcionallity: this function prints the prime components by the specified format 
 void printByFormat(int number, char* prime_factors_by_format);