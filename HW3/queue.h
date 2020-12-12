#pragma once
#include <windows.h>
#include <limits.h> 
#include <stdio.h> 
#include <stdlib.h>
#include "Functions.h"
#include "HardCodedData.h"
/// "destroy queue" was not implemented yet
typedef struct  {
    int front, rear, size;
    unsigned int capacity;
    int* array;
    HANDLE mutex_fifo; 
}Queue;
int InitializeQueue(unsigned int capacity, Queue** OUT queue);
int isFull( Queue* queue);
int isEmpty( Queue* queue);
int push( Queue* queue, int item);
int pop( Queue* queue);
int Top( Queue* queue);
int fill_fifo(Queue* queue, HANDLE  input_file, int num_of_lines);
