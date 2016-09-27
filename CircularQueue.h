#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H

#include <stdio.h>	

typedef struct CircularQueue
{
	void** arrayPtr;
	int front;
	int back;
	int itemSize;
	int capacity;
	int numElements;
}CircularQueue;

int CQ_Enqueue(CircularQueue* queue, void* itemPtr);
int CQ_Dequeue(CircularQueue* queue, void* itemOut);
int CQ_Init(CircularQueue* queue, void** arrayPtr, int itemSize, int arrayCapacity);

#endif
