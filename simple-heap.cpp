/*!
	@file 	simple-heap.cpp
	@brief	Implemented functions for a simple heap class.

	@author Bastian Rieck
*/

#include "simple-heap.h"

/*!
	Initializes the heap.

	@param max_size Allocates memory for max_size elements.
*/

simple_heap::simple_heap(unsigned int max_size)
{
	elements = new unsigned int[max_size+2];
	last = 0;

	for(unsigned int i = 0; i < (max_size+2); i++)
		elements[i] = 0;
}

/*!
	Releases memory.
*/

simple_heap::~simple_heap()
{
	delete[] elements;
}


/*!
	Adds a new element to the heap and performs the reheap operation. 

	@param x Element to add to the heap.
*/

void simple_heap::push(unsigned int x)
{
	elements[++last] = x;
	reheap_up(last);
}

/*!
	Performs the reheap condition, starting from the specified node in an
	upwards direction.

	@param start Index of node where to start the process. 
*/

void simple_heap::reheap_up(unsigned int start)
{
	unsigned int child  = start;
	unsigned int father = child/2;

	// Swap all elements in the current subtree that are not satisfying the
	// heap property.
	while(child > 1 && elements[father] > elements[child])
	{
		unsigned int tmp = elements[father];
		elements[father] = elements[child];
		elements[child] = tmp;
		
		child /= 2;
		father /= 2;
	}
}
/*!
	Performs the reheap condition, starting from the specified node in an
	downwards direction.

	@param start Index of node where to start the process.
*/

void simple_heap::reheap_down(unsigned int start)
{
	unsigned int child;
	
	// Check whether it is possible to go down further
	if(2*start > this->last)
		return;

	// Determine which child is to be visited
	if((2*start+1) <= this->last && elements[2*start+1] < elements[2*start])
		child = 2*start+1;
	else
		child = 2*start;

	// Swap nodes if necessary	
	if(elements[start] > elements[child])
	{
		unsigned int tmp = elements[start];
		elements[start] = elements[child];
		elements[child] = tmp;
		
		reheap_down(child);
	}
}
