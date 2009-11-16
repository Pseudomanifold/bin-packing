/*!
	@file	best-fit.h
	@brief	Prototypes for "Best-Fit" heuristic

	@author Bastian Rieck
*/

#ifndef BEST_FIT_H
#define BEST_FIT_H

unsigned int best_fit(const unsigned int*, unsigned int*, double&);
unsigned int best_fit_depq(const unsigned* objects, double& time);

#endif
