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
	Context(){
		mutation_rate = 0;
		config = nullptr;
		num_parents = 0;
		tournament_size = 0;
		exchange_probability = 0.5;
		num_migrants = 0;
	}

	Context& operator=(Context&);
	Context(const Context&);
public:
	static Context * instance(){
		static Context the_instance;
		return &the_instance;
	}

	Config * config;
	double mutation_rate;
	unsigned short num_parents;
	unsigned short tournament_size;
	double exchange_probability;
	int num_migrants;
};


#endif /* CONTEXT_H_ */
