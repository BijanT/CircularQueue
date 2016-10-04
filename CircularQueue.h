#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H

#include <stdio.h>	
#include <string.h>

typedef struct CircularQueue
{
	void* arrayPtr;
	unsigned int front;
	unsigned int back;
	unsigned int itemSize;
	unsigned int capacity;//The max number of elements that can be stored in the array 
	unsigned int arraySize;//The size of the array in bytes
	unsigned int numElements;
	unsigned int freeBytes;//The total number of free bytes in the array
	
}CircularQueue;

int CQ_Enqueue(CircularQueue* queue, void* itemPtr);
int CQ_EnqueueBuffer(CircularQueue* queue, void* buffPtr, int buffSize);
int CQ_Dequeue(CircularQueue* queue, void* itemOut);
int CQ_Init(CircularQueue* queue, void* arrayPtr, int itemSize, int arrayCapacity);

#endif
