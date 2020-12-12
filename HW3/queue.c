#include "queue.h"



// function to create a queue of a given capacity(number of lines in the file)
// It initializes size of queue as 0 
struct Queue* InitializeQueue(unsigned int capacity)
{
    struct Queue* queue = (struct Queue*)malloc(
        sizeof(struct Queue));
    if (NULL == queue)
    {
        printf("memory allocation failed");
        return MEMORY_ALLOCATION_FAILURE;
    }
    queue->capacity = capacity;
    queue->front = queue->size = 0;

    // This procedure is important, for the enqueue 
    queue->rear = capacity - 1;
    queue->array = (int*)malloc(
        queue->capacity * sizeof(int));
    if (NULL == queue->array) {
        printf("memory allocation failed");
        return MEMORY_ALLOCATION_FAILURE;
    }
    int ErrorValue = CreateMutexWrap(FALSE, queue->mutex_fifo);
    if (ErrorValue != SUCCESS)
        return ErrorValue;
    return queue;
}

// Queue is full when size becomes 
// equal to the capacity 
int isFull(struct Queue* queue)
{
    return (queue->size == queue->capacity);
}

// Queue is empty when size is 0 
int isEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}
// Function to add an item to the queue. 
// It changes rear and size 
void push(struct Queue* queue, int item)
{
    if (isFull(queue))
        return;
    int ErrorValue = WaitForSingleObjectWrap(queue, TIMEOUT_IN_MILLISECONDS);
    if (ErrorValue != SUCCESS)
        return ErrorValue;
    queue->rear = (queue->rear + 1)
        % queue->capacity;
    queue->array[queue->rear] = item;
    queue->size = queue->size + 1;
    printf("%d enqueued to queue\n", item);
    ErrorValue = ReleaseMutexeWrap(queue, TIMEOUT_IN_MILLISECONDS);
    if (ErrorValue != SUCCESS)
        return ErrorValue;
                                                                                                                               
}
// Function to remove an item from queue.
// It changes front and size 
int pop(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    int ErrorValue = WaitForSingleObjectWrap(queue, TIMEOUT_IN_MILLISECONDS);
    if (ErrorValue != SUCCESS)
        return ErrorValue;
    int item = queue->array[queue->front];
    queue->front = (queue->front + 1)
        % queue->capacity;
    queue->size = queue->size - 1;
    ErrorValue = ReleaseMutexeWrap(queue, TIMEOUT_IN_MILLISECONDS);
    if (ErrorValue != SUCCESS)
        return ErrorValue;
    return item;
}

// Function to get front of queue 
int Top(struct Queue* queue)
{
    if (isEmpty(queue))
        return INT_MIN;
    return queue->array[queue->front];
}