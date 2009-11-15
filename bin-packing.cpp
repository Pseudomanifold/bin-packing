/*!
	@file 	bin.cpp
	@brief	Bin-packing demo program
	
	@author Bastian Rieck 
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>

using namespace std;

unsigned int n = 0; 	///< Number of objects for current problem
unsigned int K = 0; 	///< Capacity of bins for current problem
unsigned int min_size;	///< Size of smallest object for current problem
unsigned int max_size;	///< Size of largest object for current problem

/*!
	Comparison function for unsigned integers that compares values in
	decreasing order. This function is used as an argument to heapsort.

	@param a First unsigned int
	@param b Second unsigned int

	@return +1 if the first element is smaller than the second, 0 if they
	are equal, -1 if the first element is bigger that the second one.
*/

int compare_uints(const void* a, const void* b)
{
	unsigned int x = *(reinterpret_cast<const unsigned int*>(a));
	unsigned int y = *(reinterpret_cast<const unsigned int*>(b));

	if(x < y)
		return( 1);
	else if(x == y)
		return( 0);
	else
		return(-1);
}


/*!
	Reads test data from a file. The file contains n in the first line, K
	in the second line, followed all by all volumes.

	@param filename	Filename
	
	@return Pointer to an array that contains all objects. Memory is
	allocated automatically and has to be freed by the programmer. If an
	error occurs, a NULL pointer will be returned.
*/

unsigned int* load_data(const char* filename)
{
	ifstream in(filename);
	if(!in.is_open())
		return(NULL);

	n = 0;
	K = 0;

	in >> n;
	in >> K;

	if(n == 0 || K == 0)
		return(NULL);

	unsigned int i = 0;
	unsigned int* objects = new unsigned int[n];	

	min_size = K;
	max_size = 0;

	while(in >> objects[i++])
	{
		if(objects[i-1] > max_size)
			max_size = objects[i-1];
		
		if(objects[i-1] < min_size)
			min_size = objects[i-1];
	}
	
	in.close();
	return(objects);
}

/*!
	Applies the "First-Fit" heuristic to the current problem.

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

	// If the bin is filled to more than (K-min_size), no object will fit
	// anymore. Hence, the bin is removed and treated as if it was full.
	unsigned int limit_capacity = K-min_size;

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

				/*
				// Remove the bin if it is (almost) full after
				// packing the object. 
				if(bins[j] > limit_capacity)
				{
					num_open_bins--;
					num_full_bins++;
					bins[j] = bins[num_open_bins];
					bins[num_open_bins] = 0;
				}
				*/
			
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

unsigned int first_fit_decreasing(const unsigned int* objects, unsigned int* positions, double& time)
{
	unsigned int* sorted_objects = new unsigned int[n];
	memcpy(sorted_objects, objects, n*sizeof(unsigned int));

	unsigned int num_bins;

	clock_t start = clock();
	if(heapsort(sorted_objects, n, sizeof(unsigned int), compare_uints) == 0)
		num_bins = first_fit(sorted_objects, positions, time);
	else
		cerr << "first_fit_decreasing: Heapsort failed\n";
	clock_t end = clock();

	time = (end-start)/static_cast<double>(CLOCKS_PER_SEC);
		
	delete[] sorted_objects;
	return(num_bins);
}

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
		cout << "next_fit_decreasing: Heapsort failed\n";
	
	clock_t end = clock();
	time = (end-start)/static_cast<double>(CLOCKS_PER_SEC);

	delete[] sorted_objects;
	return(num_bins);
}

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

int main(int argc, char* argv[])
{
	unsigned int* objects 	= load_data("bp4");
	unsigned int* positions = new unsigned int[n];

	cout 	<< "#objects:\t"	<< n		<< "\n"
		<< "min:\t" 		<< min_size 	<< "\n"
		<< "max:\t"		<< max_size 	<< "\n\n";

	double time;	
	
	/*
	cout << "Max-Rest: " << max_rest(objects, positions, time) << " bins, "; 
	cout << time << "s\n"; 
	*/
	
		
	cout << "First-Fit: " << first_fit(objects, positions, time) << " bins, ";
	cout << time << "s\n"; 
	/*
	cout << "First-Fit-Decreasing: " << first_fit_decreasing(objects, positions, time) << " bins, ";
	cout << time << "s\n";
	
	cout << "Next-Fit: " << next_fit(objects, positions, time) << " bins, ";
	cout << time << "s\n";

	cout << "Next-Fit-Decreasing: " << next_fit_decreasing(objects, positions, time) << " bins, "; 
	cout << time << "s\n";

	cout << "Best-Fit: " << best_fit(objects, positions, time) << " bins, ";
	cout << time << "s\n";
	*/

	delete[] objects;
	delete[] positions;
}
