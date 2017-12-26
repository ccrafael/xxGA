/*
 * Context.h
 *
 *  Created on: 8 Dec 2017
 *      Author: fali
 */

#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_

#include "Config.h"

/*
 * This is a singleton class that allow quick access to global conf parameters
 * like thresholds, rates, etc usually used by the operators.
 */
class Environment {
	Environment();
	static Environment * the_instance ;
	Environment& operator=(Environment&);
	Environment(const Environment&);
public:
	static Environment * instance() ;

	Config * config;
	double mutation_rate;
	unsigned short num_parents;
	unsigned short tournament_size;
	double exchange_probability;
	int num_offspring;
	int num_migrants;
};



#endif /* ENVIRONMENT_H_ */
