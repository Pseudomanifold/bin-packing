/*!
	@file 	best-fit.cpp
	@brief	Implementation of "Best-Fit" heuristic

	@author Bastian Rieck
*/

#include <cstring>
#include <ctime>
#include <cstdlib>

#include "bin-packing.h"

unsigned int best_fit(const unsigned* objects, unsigned int* positions, double& time)
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
		unsigned int best_bin = n; // best bin that has been determined so far
		unsigned int best_cap = 0; // capacity for said bin if the object has been added

		for(unsigned int j = 0; j < num_open_bins; j++)
		{
			unsigned int temp_cap = bins[j]+objects[i];
			if(temp_cap <= K && temp_cap > best_cap)
			{
				best_bin = j;
				best_cap = temp_cap;
			}
		}

		// Best bin has been found...
		if(best_bin < n)
		{
			bins[best_bin] += objects[i];
			positions[i] = best_bin;

			// Remove (almost) full bins
			if(bins[best_bin] > limit_capacity)
			{
				num_open_bins--;
				num_full_bins++;
				bins[best_bin] = bins[num_open_bins];
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

