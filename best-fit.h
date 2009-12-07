/*!
	@file	best-fit.h
	@brief	Prototypes for implementations of the "Best-Fit" heuristic

	@author Bastian Rieck
*/

#ifndef BEST_FIT_H
#define BEST_FIT_H

unsigned int best_fit(const unsigned int*, unsigned int*, double&);
unsigned int best_fit_heap(const unsigned* objects, double&);
unsigned int best_fit_lookup(const unsigned* objects, double&);

#endif
