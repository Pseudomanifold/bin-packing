/*!
	@file 	bin.cpp
	@brief	Bin-packing demo program
	
	@author Bastian Rieck 
*/

#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>

#include "bin-packing.h"
#include "first-fit.h"
#include "next-fit.h"
#include "best-fit.h"
#include "max-rest.h"

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
	Reads test data from STDIN. The test data is supposed to come from a
	file that contains n in the first line, K in the second line, followed
	all by all volumes.

	@return Pointer to an array that contains all objects. Memory is
	allocated automatically and has to be freed by the programmer. If an
	error occurs, a NULL pointer will be returned.
*/

unsigned int* load_data()
{
	n = 0;
	K = 0;

	cin >> n;
	cin >> K;

	if(n == 0 || K == 0)
		return(NULL);

	unsigned int i = 0;
	unsigned int* objects = new unsigned int[n];	

	min_size = K;
	max_size = 0;

	while(cin >> objects[i++])
	{
		if(objects[i-1] > max_size)
			max_size = objects[i-1];
		
		if(objects[i-1] < min_size)
			min_size = objects[i-1];
	}
	
	return(objects);
}

int main(int argc, char* argv[])
{
	unsigned int* objects 	= load_data();
	unsigned int* positions = new unsigned int[n];

	cout 	<< "#objects:\t"	<< n		<< "\n"
		<< "min:\t" 		<< min_size 	<< "\n"
		<< "max:\t"		<< max_size 	<< "\n\n";

	double time;	

	cout << "Max-Rest: " << max_rest(objects, positions, time) << " bins, "; 
	cout << time << "s\n"; 

	cout << "First-Fit: " << first_fit(objects, positions, time) << " bins, ";
	cout << time << "s\n"; 
	
	cout << "First-Fit-Decreasing: " << first_fit_decreasing(objects, positions, time) << " bins, ";
	cout << time << "s\n";

	cout << "Next-Fit: " << next_fit(objects, positions, time) << " bins, ";
	cout << time << "s\n";

	cout << "Next-Fit-Decreasing: " << next_fit_decreasing(objects, positions, time) << " bins, "; 
	cout << time << "s\n";

	cout << "Best-Fit: " << best_fit(objects, positions, time) << " bins, ";
	cout << time << "s\n";

	delete[] objects;
	delete[] positions;
}
