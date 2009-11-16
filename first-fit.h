/*!
	@file 	first-fit.h
	@brief	Prototypes for "First-Fit" and "First-Fit-Decreasing"
		heuristics.
	
	@author Bastian Rieck
*/

#ifndef FIRST_FIT_H
#define FIRST_FIT_H

unsigned int first_fit(const unsigned int*, unsigned int*, double&);
unsigned int first_fit_decreasing(const unsigned int*, unsigned int*, double&);

unsigned int first_fit_vec(const unsigned int*, double&);
unsigned int first_fit_decreasing_vec(	const unsigned int*, 
					double&, 
					int(*)(void*, size_t, size_t, int (*)(const void*, const void*)));

#endif
