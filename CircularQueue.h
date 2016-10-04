#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H

#include <stdio.h>	
#include <string.h>
#include <stdint.h>

typedef struct CircularQueue
{
	void* arrayPtr;
	uint32_t front;
	uint32_t back;
	uint32_t itemSize;
	uint32_t capacity;//The max number of elements that can be stored in the array 
	uint32_t arraySize;//The size of the array in bytes
	uint32_t numElements;
	uint32_t freeBytes;//The total number of free bytes in the array
	
}CircularQueue;

int CQ_Enqueue(CircularQueue* queue, void* itemPtr);
int CQ_EnqueueBuffer(CircularQueue* queue, void* buffPtr, int buffSize);
int CQ_Dequeue(CircularQueue* queue, void* itemOut);
int CQ_Init(CircularQueue* queue, void* arrayPtr, int itemSize, int arrayCapacity);

#endif
