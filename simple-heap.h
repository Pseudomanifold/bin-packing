/*!
	@file 	simple-heap.h
	@brief	Simple heap implementation

	@author Bastian Rieck
*/

#ifndef SIMPLE_HEAP_H
#define SIMPLE_HEAP_H

/*!
	Describes a heap based on unsigned integers. Only the most basic
	operations have been implemented.
*/

class simple_heap {
	public:
		simple_heap(unsigned int max_size);
		~simple_heap();

		void push(unsigned int item);
		
		unsigned int* elements;
		unsigned int last;

		void reheap_up(unsigned int start);
		void reheap_down(unsigned int ptr);
};

#endif
