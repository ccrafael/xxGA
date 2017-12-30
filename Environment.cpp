/*
 * Context.cpp
 *
 *  Created on: 16 Dec 2017
 *      Author: fali
 */

#include "Environment.h"

Environment * Environment::the_instance = nullptr;

Environment::Environment () {
		mutation_rate = 0.5;
		config = nullptr;
		tournament_size = 10;
		exchange_probability = 0.5;
		num_offspring = 0;
		mutation_prop = 0;
	}


Environment * Environment::instance() {
		if (the_instance == nullptr) {
			the_instance = new Environment();
		}
		return the_instance;
	}
