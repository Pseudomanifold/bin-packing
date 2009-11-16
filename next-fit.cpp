/*!
	@file 	next-fit.cpp
	@brief	Contains the "Next-Fit" and "Next-Fit-Decreasing" heuristics

	@author Bastian Rieck
*/

#include <cstring>
#include <ctime>
#include <cstdlib>

#include "bin-packing.h"

/*!
	Applies the "Next-Fit" heuristic to the current problem. Worst-case
	running-time is O(n) because the heuristic loops once over the object
	set and can open new bins in constant time.
*/

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

/*!
	Applies the "Next-Fit-Decreasing" heuristic to the current problem.
	Since the worst-case running time of "Next-Fit" is O(n), the running
	time of "Next-Fit-Decreasing" is dominated by the sorting procedure.
	Hence, the worst-case running time is O(n log n) when using heapsort or
	O(n) when using csort.
*/

unsigned int next_fit_decreasing(	const unsigned* objects,
					unsigned int* positions,
					double& time,
					int (*sort)(void*, size_t, size_t, int (*)(const void*, const void*)))
{
	unsigned int* sorted_objects = new unsigned int[n];
	memcpy(sorted_objects, objects, n*sizeof(unsigned int));

	unsigned int num_bins;

	clock_t start = clock();
	if(sort(sorted_objects, n, sizeof(unsigned int), compare_uints) == 0)
		num_bins = next_fit(sorted_objects, positions, time);
	else
		return(0);

	clock_t end = clock();
	time = (end-start)/static_cast<double>(CLOCKS_PER_SEC);

	delete[] sorted_objects;
	return(num_bins);
}
