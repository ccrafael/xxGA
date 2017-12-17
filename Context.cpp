/*
 * Context.cpp
 *
 *  Created on: 16 Dec 2017
 *      Author: fali
 */

#include "Context.h"

Context * Context::the_instance = nullptr;

Context::Context () {
		mutation_rate = 0.5;
		config = nullptr;
		num_parents = 2;
		tournament_size = 10;
		exchange_probability = 0.5;
		num_migrants = 5;
	}


Context * Context::instance(){
		if (the_instance == nullptr) {
			the_instance = new Context();
		}
		return the_instance;
	}
