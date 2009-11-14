/*!
	@file 	bin.cpp
	@brief	Bin-packing demo program
	
	@author Bastian Rieck 
*/

#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

unsigned int n = 0; ///< Number of objects for current problem
unsigned int K = 0; ///< Capacity of bins for current problem

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

	while(in >> objects[i++]);
	
	in.close();
	return(objects);
}

/*!
	Applies the "First-Fit" heuristic to the current problem.

	@param objects 		Array of object sizes
	@param positions	Array that will contain the associations for the
				objects. The entry in position i signifies the bin
				that contains object i.

	@return	Number of bins opened by the heuristic.
*/

unsigned int first_fit(const unsigned int* objects, unsigned int* positions)
{
	unsigned int num_open_bins = 1;
	unsigned int num_full_bins = 0;
	unsigned int* bins = new unsigned int[n];
	
	memset(bins, 0, n*sizeof(unsigned int));
	memset(positions, 0, n*sizeof(unsigned int));

	for(unsigned int i = 0; i < n; i++)
	{
		for(unsigned int j = 0; j < num_open_bins; j++)
		{
			if((bins[j]+objects[i]) <= K)
			{
				bins[j] += objects[i];
				positions[i] = j+1;

				// Full bins need not be searched
				if(bins[j] == K)
				{
					num_full_bins++;
					num_open_bins--;
					bins[j] = bins[num_open_bins];
				}

				break;
			}
		}

		// Object could not be placed--create a new bin and put it in
		// there
		if(positions[i] == 0)
		{
			bins[num_open_bins] = objects[i];
			positions[i] = ++num_open_bins;
		}
	}

	delete[] bins;
	return(num_open_bins+num_full_bins);
}

unsigned int first_fit_decreasing(const unsigned int* objects, unsigned int* positions)
{
	unsigned int* sorted_objects = new unsigned int[n];
	memcpy(sorted_objects, objects, n*sizeof(unsigned int));

	unsigned int num_bins;
	if(heapsort(sorted_objects, n, sizeof(unsigned int), compare_uints) == 0)
		num_bins = first_fit(sorted_objects, positions);
	else
		cerr << "first_fit_decreasing: Heapsort failed\n";
	
	delete[] sorted_objects;
	return(num_bins);
}

unsigned int next_fit(const unsigned int* objects, unsigned int* positions)
{
	unsigned int cur_bin = 0; 
	unsigned int* bins = new unsigned int[n];
	
	memset(bins, 0, n*sizeof(unsigned int));
	memset(positions, 0, n*sizeof(unsigned int));

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

	delete[] bins;
	return(cur_bin+1);
}

unsigned int next_fit_decreasing(const unsigned* objects, unsigned int* positions)
{
	unsigned int* sorted_objects = new unsigned int[n];
	memcpy(sorted_objects, objects, n*sizeof(unsigned int));

	unsigned int num_bins;
	if(heapsort(sorted_objects, n, sizeof(unsigned int), compare_uints) == 0)
		num_bins = next_fit(sorted_objects, positions);
	else
		cout << "next_fit_decreasing: Heapsort failed\n";
	
	delete[] sorted_objects;
	return(num_bins);
}

unsigned int best_fit(const unsigned* objects, unsigned int* positions)
{
	unsigned int num_bins = 1;
	unsigned int* bins = new unsigned int[n];
	
	memset(bins, 0, n*sizeof(unsigned int));
	memset(positions, 0, n*sizeof(unsigned int));

	for(unsigned int i = 0; i < n; i++)
	{
		unsigned int best_bin = n; // best bin that has been determined so far
		unsigned int best_cap = 0; // capacity for said bin if the object has been added

		for(unsigned int j = 0; j < num_bins; j++)
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
		}

		// ...else create a new one
		else
		{
			bins[num_bins] = objects[i];
			positions[i] = num_bins++;
		}
	}

	delete[] bins;
	return(num_bins);

}

unsigned int max_rest(const unsigned* objects, unsigned int* positions)
{
	unsigned int num_bins = 1;
	unsigned int* bins = new unsigned int[n];
	
	memset(bins, 0, n*sizeof(unsigned int));
	memset(positions, 0, n*sizeof(unsigned int));

	for(unsigned int i = 0; i < n; i++)
	{
		unsigned int max_bin = n; 	// bin with maximum _remaining_ capacity 
		unsigned int max_rem_cap = K; 	// capacity for said bin 

		for(unsigned int j = 0; j < num_bins; j++)
		{
			if(bins[j] < max_rem_cap)
			{
				max_rem_cap = bins[j];
				max_bin	= j;
			}
		}
		
		// Check whether object fits into the bin with maximum
		// remaining capacity...
		if((bins[max_bin]+objects[i]) <= K)
		{
			bins[max_bin] += objects[i];
			positions[i] = max_bin;
		}

		// ...else create a new one
		else
		{
			bins[num_bins] = objects[i];
			positions[i] = num_bins++;
		}
	}

	delete[] bins;
	return(num_bins);

}

int main(int argc, char* argv[])
{
	unsigned int* objects 	= load_data("bp3");
	unsigned int* positions = new unsigned int[n];

	//cout << "Max-Rest: " << max_rest(objects, positions) << "\n"; 
	cout << "First-Fit: " << first_fit(objects, positions) << "\n";
	//cout << "First-Fit-Decreasing: " << first_fit_decreasing(objects, positions) << "\n";
	//cout << "Next-Fit: " << next_fit(objects, positions) << "\n";
	//cout << "Next-Fit-Decreasing: " << next_fit_decreasing(objects, positions) << "\n"; 
	//cout << "Best-Fit: " << best_fit(objects, positions) << "\n"; 

	delete[] objects;
	delete[] positions;
}
