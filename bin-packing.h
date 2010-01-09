/*!
	@file 	bin-packing.h
	@brief 	Shared prototypes and global variables

	@author Bastian Rieck
*/

#ifndef BIN_PACKING_H
#define BIN_PACKING_H

extern unsigned int n;
extern unsigned int K;
extern unsigned int min_size;
extern unsigned int max_size;

int compare_uints(const void*, const void*);
void csort(void*, size_t, size_t, int (*)(const void*, const void*));

#endif
