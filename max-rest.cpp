/*!
	@file 	max-rest.cpp
	@brief	Implementation of "Max-Rest" heuristic

	@author Bastian Rieck
*/

#include <cstring>
#include <ctime>
#include <cstdlib>

#include "bin-packing.h"

unsigned int max_rest(const unsigned* objects, unsigned int* positions, double& time)
{
	unsigned int num_open_bins = 1;
	unsigned int num_full_bins = 0;
	unsigned int* bins = new unsigned int[n];
	
	memset(bins, 0, n*sizeof(unsigned int));
	memset(positions, 0, n*sizeof(unsigned int));

	unsigned int limit_capacity = K-min_size;

	clock_t start = clock();
	for(unsigned int i = 0; i < n; i++)
	{
		unsigned int max_bin = n; 	// bin with maximum _remaining_ capacity 
		unsigned int max_rem_cap = K; 	// capacity for said bin 

		for(unsigned int j = 0; j < num_open_bins; j++)
		{
			if(bins[j] < max_rem_cap)
			{
				max_rem_cap = bins[j];
				max_bin	= j;
			}
		}
		
		// Check whether object fits into the bin with maximum
		// remaining capacity...
		if((max_bin < n) && (bins[max_bin]+objects[i]) <= K)
		{
			bins[max_bin] += objects[i];
			positions[i] = max_bin;

			// Remove (almost) full bins
			if(bins[max_bin] > limit_capacity)
			{
				num_open_bins--;
				num_full_bins++;
				bins[max_bin] = bins[num_open_bins];
			}
		}

		// ...else create a new one
		else
		{
			bins[num_open_bins] = objects[i];
			positions[i] = num_open_bins++;
		}
	}

	clock_t end = clock();
	time = (end-start)/static_cast<double>(CLOCKS_PER_SEC);

	delete[] bins;
	return(num_open_bins+num_full_bins);
}
