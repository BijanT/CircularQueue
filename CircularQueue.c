#include "CircularQueue.h"

//Initialize a new Circular Queue
int CQ_Init(CircularQueue* queue, void** arrayPtr, int itemSize, int arrayCapacity)
{	
	(*queue).arrayPtr = arrayPtr;
	(*queue).front = (*queue).back = 0;
	(*queue).itemSize = itemSize;
	(*queue).capacity = arrayCapacity;
	(*queue).numElements = 0;	

	return 1;
}

//Add a new element to the back of the queue
int CQ_Enqueue(CircularQueue* queue, void* itemPtr)
{
	//Check for null inputs
	if(queue == NULL || itemPtr == NULL)
	{
		return 0;
	}
	
	//Make sure the back is within the bounds of the array
	if((*queue).back < 0 || (*queue).back >= (*queue).capacity)
	{
		return 0;
	}

	//If the number of elements is equal to the capacity of the queue, there is nowhere
	//to put the new item, so return 0
	if((*queue).numElements == (*queue).capacity)
	{
		return 0;
 	}

	//put the value of new item in the back of the queue
	(*queue).arrayPtr[(*queue).back] = itemPtr;
	
	//update the back of the queue and increment numElements
	//Make sure the value for the back doesn't go past the size of the array
	if((*queue).back == (*queue).capacity - 1)
	{
		(*queue).back = 0;
	}
	else
	{
		(*queue).back += 1;
	}
	(*queue).numElements +=1;	

	//return 1 to indicate success
	return 1;
}

//Remove an element from the front of the queue
int CQ_Dequeue(CircularQueue* queue, void* itemOut)
{
	//Check for null inputs
	if(queue == NULL)
	{
		return 0;
	}

	//Make sure the front is within the bounds
	if((*queue).front < 0 || (*queue).front >= (*queue).capacity)
	{
		return 0;
	}

	//Grab the item in the front of the queue
	itemOut = (*queue).arrayPtr[(*queue).front];

	//Update the front and decrement numElements
	//Make sure the value for the front doesn't go past the size of the array
	if((*queue).front == (*queue).capacity - 1)
	{
		(*queue).front = 0;
	}
	else
	{
		(*queue).front += 1;
	}
	(*queue).numElements -= 1;

	return 1;
}








