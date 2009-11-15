/*!
	@file 	next-fit.cpp
	@brief	Contains the "Next-Fit" and "Next-Fit-Decreasing" heuristics

	@author Bastian Rieck
*/

#include <cstring>
#include <ctime>
#include <cstdlib>

#include "bin-packing.h"

unsigned int next_fit(const unsigned int* objects, unsigned int* positions, double& time)
{
	unsigned int cur_bin = 0; 
	unsigned int* bins = new unsigned int[n];
	
	memset(bins, 0, n*sizeof(unsigned int));
	memset(positions, 0, n*sizeof(unsigned int));

	clock_t start = clock();
	for(unsigned int i = 0; i < n; i++)
	{
		// Check whether the object fits in the current bin... 
		if((bins[cur_bin]+objects[i]) <= K)
			bins[cur_bin] += objects[i];

		// ...else open a new one
		else
			bins[++cur_bin] += objects[i];
		
		positions[i] = cur_bin;
	}
	
	clock_t end = clock();
	time = (end-start)/static_cast<double>(CLOCKS_PER_SEC);

	delete[] bins;
	return(cur_bin+1);
}

unsigned int next_fit_decreasing(const unsigned* objects, unsigned int* positions, double& time)
{
	unsigned int* sorted_objects = new unsigned int[n];
	memcpy(sorted_objects, objects, n*sizeof(unsigned int));

	unsigned int num_bins;
	
	clock_t start = clock();
	if(heapsort(sorted_objects, n, sizeof(unsigned int), compare_uints) == 0)
		num_bins = next_fit(sorted_objects, positions, time);
	else
		return(0);
	
	clock_t end = clock();
	time = (end-start)/static_cast<double>(CLOCKS_PER_SEC);

	delete[] sorted_objects;
	return(num_bins);
}