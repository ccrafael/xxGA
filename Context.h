/*
 * Context.h
 *
 *  Created on: 8 Dec 2017
 *      Author: fali
 */

#ifndef CONTEXT_H_
#define CONTEXT_H_

#include "Config.h"

/*
 * This is a singleton class that allow quick access to global conf parameters
 * like thresholds, rates, etc usually used by the operators.
 */
class Context {
	Context();
	static Context * the_instance ;
	Context& operator=(Context&);
	Context(const Context&);
public:
	static Context * instance() ;

	Config * config;
	double mutation_rate;
	unsigned short num_parents;
	unsigned short tournament_size;
	double exchange_probability;
	int num_migrants;

};



#endif /* CONTEXT_H_ */
