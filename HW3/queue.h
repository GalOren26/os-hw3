#pragma once
#include <windows.h>

struct Queue* createQueue(unsigned int capacity);
int isFull(struct Queue* queue);
int isEmpty(struct Queue* queue);
void enqueue(struct Queue* queue, int item);
int pop(struct Queue* queue);
int front(struct Queue* queue);
