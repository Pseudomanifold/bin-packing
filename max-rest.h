/*!
	@file	max-rest.h
	@brief	Prototype for "Max-Rest" heuristic

	@author Bastian Rieck
*/

#ifndef MAX_REST_H
#define MAX_REST_H

unsigned int max_rest(const unsigned int*, unsigned int*, double&);
unsigned int max_rest_pq(const unsigned int* objects, double& time);

#endif
