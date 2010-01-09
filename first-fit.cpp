/*!
	@file 	first-fit.cpp
	@brief 	Contains "First-Fit" and "First-Fit-Decreasing" heuristics.

	@author Bastian Rieck
*/

#include <iostream>
#include <cstring>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <map>

#include "bin-packing.h"

/*!
	Applies the "First-Fit" heuristic to the current problem. Worst-case
	running time is O(n^2). 

	@param objects 		Array of object sizes
	@param positions	Array that will contain the associations for the
				objects. The entry in position i signifies the bin
				that contains object i.
	@param time		Variable that will be filled with the elapsed time
				after the bins haven been sorted.

	@return	Number of bins opened by the heuristic.
*/

unsigned int first_fit(const unsigned int* objects, unsigned int* positions, double& time)
{
	unsigned int num_open_bins = 1;
	unsigned int num_full_bins = 0;
	unsigned int* bins = new unsigned int[n];

	memset(bins, 0, n*sizeof(unsigned int));
	memset(positions, 0, n*sizeof(unsigned int));

	// This variable is set per object. It is the minimum capacity that is
	// required in order to fit the object into a certain bin.
	unsigned int required_capacity;

	clock_t start = clock();
	for(unsigned int i = 0; i < n; i++)
	{
		required_capacity = K-objects[i];
		for(unsigned int j = 0; j < num_open_bins; j++)
		{
			if(bins[j] <= required_capacity)
			{
				bins[j] += objects[i];
				positions[i] = j+1;

				break;
			}
		}

		// Object could not be placed--create a new bin and put it in
		// there
		if(positions[i] == 0)
		{
			bins[num_open_bins] = objects[i];
			positions[i] = (++num_open_bins);
		}
	}
	
	clock_t end = clock();
	time = (end-start)/static_cast<double>(CLOCKS_PER_SEC);

	delete[] bins;
	return(num_open_bins+num_full_bins);
}

/*
	Applies the "First-Fit-Decreasing" heuristic to the current problem.
	Worst-case running time is, not taking into account sorting with O(n
	log n), O(n^2) because the "First-Fit" heuristic is called.
*/

unsigned int first_fit_decreasing(const unsigned int* objects, unsigned int* positions, double& time)
{
	unsigned int* sorted_objects = new unsigned int[n];
	memcpy(sorted_objects, objects, n*sizeof(unsigned int));

	unsigned int num_bins;

	clock_t start = clock();
	qsort(sorted_objects, n, sizeof(unsigned int), compare_uints); 
	num_bins = first_fit(sorted_objects, positions, time);
	clock_t end = clock();

	time = (end-start)/static_cast<double>(CLOCKS_PER_SEC);
		
	delete[] sorted_objects;
	return(num_bins);
}

/*!
	Implementation of First-Fit heuristic using STL vectors. This allows us
	to remove bins that are almost full without disturbing the order of
	other elements. The positions array is not used in this implementation.
*/

unsigned int first_fit_vec(const unsigned int* objects, double& time)
{
	unsigned int num_open_bins = 1;
	unsigned int num_full_bins = 0;

	std::vector<unsigned int> bins;
	bins.push_back(0);

	// If the bin is filled to more than (K-min_size), no object will fit
	// anymore. Hence, the bin is removed and treated as if it was full.
	unsigned int limit_capacity = K-min_size;

	// This variable is set per object. It is the minimum capacity that is
	// required in order to fit the object into a certain bin.
	unsigned int required_capacity;

	// Flag that signals whether the current object could be placed in an
	// existing bin or not.
	bool placed;

	std::vector<unsigned int>::iterator last_bin;
	std::vector<unsigned int>::iterator bin;
	
	clock_t start = clock();
	for(unsigned int i = 0; i < n; i++)
	{
		required_capacity = K-objects[i];
		placed = false;

		// The previous object was smaller and could _not_ be placed in
		// a bin with smaller index. It follows that we can skip the
		// other bins.
		if(i >= 1 && objects[i] >= objects[i-1])
			bin = last_bin;
		else
			bin = bins.begin();
		
		for(; bin != bins.end(); bin++)
		{
			if(*bin <= required_capacity)
			{
				*bin += objects[i];
				last_bin = bin;

				if(*bin > limit_capacity)
				{
					num_open_bins--;
					num_full_bins++;
					bins.erase(bin);
				}

				placed = true;
				break;
			}
		}

		if(!placed) 
		{
			bins.push_back(objects[i]);
			num_open_bins++;

			last_bin = bins.end()-1;
		}
	}
	
	clock_t end = clock();
	time = (end-start)/static_cast<double>(CLOCKS_PER_SEC);

	return(num_open_bins+num_full_bins);
}

/*!
	Applies the "First-Fit-Decreasing" heuristic to the current problem
	using the STL vector class. Since first_fit_vec is used, no positions
	array is required.
*/

unsigned int first_fit_decreasing_vec(	const unsigned int* objects,
					double& time,
					void(*sort)(void*, size_t, size_t, int (*)(const void*, const void*)))
{
	unsigned int* sorted_objects = new unsigned int[n];
	memcpy(sorted_objects, objects, n*sizeof(unsigned int));

	unsigned int num_bins;

	clock_t start = clock();
	sort(sorted_objects, n, sizeof(unsigned int), compare_uints); 
	num_bins = first_fit_vec(sorted_objects, time);
	clock_t end = clock();

	time = (end-start)/static_cast<double>(CLOCKS_PER_SEC);
		
	delete[] sorted_objects;
	return(num_bins);
}

/*!
	Implementation of First-Fit heuristic using STL maps, thus speeding up
	the process of looking for a suitable bin.  The positions array is not
	used in this implementation.
*/

unsigned int first_fit_map(const unsigned int* objects, double& time)
{
        unsigned int num_open_bins = 1;
        unsigned int num_full_bins = 0;
        unsigned int* bins = new unsigned int[n];

        memset(bins, 0, n*sizeof(unsigned int));

	// The map associates an object weight to an index in the bin array.
	// The index signifies the minimum index at which an object of the
	// current weight could be placed the last time. All bins with smaller
	// index need not be checked.
        std::map<unsigned int, unsigned int> bin_map;

        // This variable is set per object. It is the minimum capacity that is
        // required in order to fit the object into a certain bin.
        unsigned int required_capacity;

	// Flag that signals whether the current object could be placed in an
	// existing bin or not.
	bool placed;

        clock_t start = clock();
        for(unsigned int i = 0; i < n; i++)
        {
                placed = false;
                
		required_capacity = K-objects[i];
                for(unsigned int j = bin_map[objects[i]]; j < num_open_bins; j++)
                {
                        if(bins[j] <= required_capacity)
                        {
                                bins[j] += objects[i];
                                bin_map[objects[i]] = j;

                                placed = true;
                                break;
                        }
                }

                // Object could not be placed--create a new bin and put it in
                // there
                if(!placed)
                {
                        bins[num_open_bins] = objects[i];
                        bin_map[objects[i]] = num_open_bins++;
                }
        }

        clock_t end = clock();
        time = (end-start)/static_cast<double>(CLOCKS_PER_SEC);

        delete[] bins;
        return(num_open_bins+num_full_bins);
}

/*!
	Applies the "First-Fit-Decreasing" heuristic to the current problem
	using the STL map class. 
*/

unsigned int first_fit_decreasing_map(	const unsigned int* objects,
					double& time,
					void(*sort)(void*, size_t, size_t, int (*)(const void*, const void*)))
{
	unsigned int* sorted_objects = new unsigned int[n];
	memcpy(sorted_objects, objects, n*sizeof(unsigned int));

	unsigned int num_bins;

	clock_t start = clock();
	sort(sorted_objects, n, sizeof(unsigned int), compare_uints); 
	num_bins = first_fit_map(sorted_objects, time);
	clock_t end = clock();

	time = (end-start)/static_cast<double>(CLOCKS_PER_SEC);
		
	delete[] sorted_objects;
	return(num_bins);
}

