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
		num_parents = 2;
		tournament_size = 10;
		exchange_probability = 0.5;
		num_migrants = 5;
	}


Environment * Environment::instance(){
		if (the_instance == nullptr) {
			the_instance = new Environment();
		}
		return the_instance;
	}
