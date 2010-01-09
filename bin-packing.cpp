/*!
*	@file	bin.cpp
*	@brief	Bin-packing demo program
*
*	@mainpage Bin-packing demo
*
*	An implementation of some of the most common heuristics for the Bin-packing
*	problem. The algorithms can be compared using demo data.
*
*	<HR>
*
* 	Copyright 2010, Bastian Rieck. All rights reserved.
*
* 	Redistribution and use in source and binary forms, with or without
* 	modification, are permitted provided that the following conditions are
* 	met:
*
* 	-# Redistributions of source code must retain the above copyright
* 	notice, this list of conditions and the following disclaimer.
*	-# Redistributions in binary form must reproduce the above copyright
*	notice, this list of conditions and the following disclaimer in the
*	documentation and/or other materials provided with the distribution.
*
*	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
*	IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
*	TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
*	PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
*	OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
*	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
*	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*	<HR>
*
*	@author Bastian Rieck
*	@date 	January 2010
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <ctime>

#include "bin-packing.h"
#include "first-fit.h"
#include "next-fit.h"
#include "best-fit.h"
#include "max-rest.h"

using namespace std;

unsigned int n = 0;	///< Number of objects for current problem
unsigned int K = 0;	///< Capacity of bins for current problem
unsigned int min_size;	///< Size of smallest object for current problem
unsigned int max_size;	///< Size of largest object for current problem
unsigned int sum_size;	///< Sum of object sizes for current problem

unsigned int* objects;		///< Array that holds the object sizes
unsigned int* positions;	///< Array that holds the object positions (not used for every heuristic)

/*!
	Comparison function for unsigned integers that compares values in
	decreasing order. This function is used as an argument to qsort.

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
	Implementation of counting sort for unsigned integers. The numbers will
	be sorted in decreasing order. The function has the same signature as
	the qsort and heapsort methods for the standard C library, thus
	allowing the routines to switch the sorting method.

	@param base	Pointer to objects array
	@param nmemb	Unused
	@param size	Unused; size is known by global variable n
	@param compar	Unused
*/

void csort(void* base, size_t nmemb, size_t size, int (*compar)(const void*, const void*))
{
	unsigned int range = max_size - min_size + 1;
	unsigned int* count = new unsigned int[range];

	memset(count, 0, range*sizeof(unsigned int));

	unsigned int* objects = reinterpret_cast<unsigned int*>(base);

	for(unsigned int i = 0; i < n; i++)
		count[objects[i] - min_size]++;

	unsigned int z = 0;
	for(unsigned int i = min_size; i <= max_size; i++)
	{
		for(unsigned int j = 0; j < count[i - min_size]; j++)
			objects[n-1-z++] = i;
	}

	delete []count;
}

/*!
	Reads test data from STDIN. The test data is supposed to come from a
	file that contains n in the first line, K in the second line, followed
	by all volumes.

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

		sum_size += objects[i-1];	
	}

	return(objects);
}

/*!
	Writes the results of running a given heuristic to the screen. The
	output will be formatted.

	@param name	Name of the heuristic
	@param num_bins Number of bins opened by heuristic
	@param time	Running time of the heuristic
*/

void output_results(const char* name, unsigned int num_bins, double time)
{
	cout << setw(30) << left << name << "";
	cout << setw( 8) << right << num_bins << " bins, ";
	cout << fixed << setprecision(2) << (100.0*(num_bins/(sum_size/static_cast<double>(K)))) << "% max. deviation, ";
	cout << fixed << setprecision(4) << time << "s\n";
}

/*!
	Runs a certain heuristic on the current test data and formats the
	output.

	Overloadeded versions of this function exist.

	@param name 	Name of the heuristic
	@param f	Function pointer to heuristic
*/

void run(const char* name, unsigned int (*f)(const unsigned int*, unsigned int*, double&))
{
	double time;
	unsigned int num_bins = f(objects, positions, time);

	output_results(name, num_bins, time);
}

void run(const char* name, unsigned int (*f)(const unsigned int*, double&))
{
	double time;
	unsigned int num_bins = f(objects, time);
	
	output_results(name, num_bins, time);
}

void run(const char* name, unsigned int (*f)(	const unsigned int*,
						double&,
						void(*)(void*, size_t, size_t, int (*)(const void*, const void*))),
						void(*sort)(void*, size_t, size_t, int (*)(const void*, const void*)))
{
	double time;
	unsigned int num_bins = f(objects, time, sort);
	
	output_results(name, num_bins, time);
}

/*!
	Runs all heuristics for the current problem, including any SLOW
	implementations.
*/

void run_all()
{
	run("Max-Rest:", 			max_rest);
	run("Max-Rest+:", 			max_rest_pq);
	run("First-Fit:", 			first_fit);
	run("First-Fit+:", 			first_fit_vec);
	run("First-Fit++:",			first_fit_map);
	run("First-Fit-Decreasing:", 		first_fit_decreasing);
	run("First-Fit-Decreasing+ (HS):", 	first_fit_decreasing_vec, qsort);
	run("First-Fit-Decreasing+ (CS):", 	first_fit_decreasing_vec, csort);
	run("First-Fit-Decreasing++:", 		first_fit_decreasing_map, csort);
	run("Next-Fit:", 			next_fit);
	run("Next-Fit-Decreasing:", 		next_fit_decreasing, qsort);
	run("Next-Fit-Decreasing+:", 		next_fit_decreasing, csort);
	run("Best-Fit:", 			best_fit);
	run("Best-Fit+:", 			best_fit_heap);
	run("Best-Fit++:", 			best_fit_lookup);
}

/*!
	Runs only the fastest heuristics for the current problem (default setting).
*/

void run_fastest()
{
	run("Max-Rest+:", 			max_rest_pq);
	run("First-Fit++:",			first_fit_map);
	run("First-Fit-Decreasing++:", 		first_fit_decreasing_map, csort);
	run("Next-Fit:", 			next_fit);
	run("Next-Fit-Decreasing+:", 		next_fit_decreasing, csort);
	run("Best-Fit++:", 			best_fit_lookup);
}

int main(int argc, char* argv[])
{
	objects = load_data();
	positions = new unsigned int[n];

	cout 	<< "****************************************\n"
		<< "* COMPARISON OF BIN-PACKING HEURISTICS *\n"
		<< "****************************************\n\n"
		<< "Objects:      " << n << "\n"
		<< "Minimum size: " << min_size << "\n"
		<< "Maximum size: " << max_size << "\n"
		<< "Sum of sizes: " << sum_size << "\n"
		<< "Bin capacity: " << K << "\n\n";

	if(getopt(argc, argv, "a") == 'a')
		run_all();
	else
		run_fastest();

	delete[] objects;
	delete[] positions;
}
