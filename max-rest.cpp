/*!
	@file	max-rest.cpp
	@brief	Implementation of "Max-Rest" heuristic

	@author Bastian Rieck
*/

#include <cstring>
#include <ctime>
#include <cstdlib>
#include <queue>

#include "bin-packing.h"

/*!
	Performs the "Max-Rest" heuristic for the current problem. Worst-case
	running time is O(n^2).
*/

unsigned int max_rest(const unsigned int* objects, unsigned int* positions, double& time)
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
		unsigned int max_bin = n;	// bin with maximum _remaining_ capacity
		unsigned int max_rem_cap = K;	// capacity for said bin

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


/*!
	Performs the "Max-Rest" heuristic using priority queue for bin
	selection. In order to facilitate the algorithm, the positions of
	objects are not saved.
*/

unsigned int max_rest_pq(const unsigned int* objects, double& time)
{
	unsigned int num_open_bins = 1;
	unsigned int num_full_bins = 0;

	// The "greater" comparison function is used so that smaller elements
	// (i.e. bins with _greater_ remaining capacity) are preferred. The
	// queue is initialized using a single element which corresponds to 1
	// empty bin.
	std::priority_queue<unsigned int, std::vector<unsigned int>, std::greater<unsigned int > > pq;
	pq.push(0);

	unsigned int limit_capacity = K-min_size;
	unsigned int bin = 0;

	clock_t start = clock();
	for(unsigned int i = 0; i < n; i++)
	{
		// No more bins available, make sure that a new one
		// is created.
		if(pq.empty())
			bin = K;
		else
			bin = pq.top();

		if((bin + objects[i]) <= K)
		{
			bin += objects[i];
			pq.pop();	// always remove the bin; it will be added later
					// on if its capacity is sufficiently large

			if(bin > limit_capacity)
			{
				num_open_bins--;
				num_full_bins++;
			}
			else
				pq.push(bin);
		}

		// Create a new bin by pushing the object size to the priority
		// queue
		else
		{
			num_open_bins++;
			pq.push(objects[i]);
		}
	}

	clock_t end = clock();
	time = (end-start)/static_cast<double>(CLOCKS_PER_SEC);

	return(num_open_bins+num_full_bins);
}
