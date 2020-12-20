#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <time.h>
#include "../HW3/Functions.h"
#include "../HW3/HardCodedData.h"

void swap(int* a,  int* b);
void randomize(int arr[], int n);
void generate_array_tests(int test_array[], int test_priorities[]);
int write_to_tasks_file(HANDLE tasks_file, int* test_array[]);
int write_to_priorities_file(HANDLE priorities_file, int test_priorities[]);