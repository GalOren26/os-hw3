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
 Queue* InitializeQueue(unsigned int capacity);
int isFull( Queue* queue);
int isEmpty( Queue* queue);
void push( Queue* queue, int item);
int pop( Queue* queue);
int Top( Queue* queue);

