/*
 * MutationCollection.h
 *
 *  Created on: 8 Dec 2017
 *      Author: fali
 */

#ifndef OPERATORS_MUTATIONCOLLECTION_H_
#define OPERATORS_MUTATIONCOLLECTION_H_

#include <functional>
#include <random>
#include "../IContainer.h"
#include "../Individual.h"
#include "../GenotypeBit.h"
#include "../exception/OperatorException.h"
#include "../Environment.h"

namespace mutation {

/**
 * Do nothing.
 */
const std::function<void(IContainer*)> empty = [](IContainer* i) {
	// do what was promised
	};

/**
 * Do nothing.
 */
const std::function<void(IContainer*)> rate = [](IContainer* g) {
	Environment* context = Environment::instance();

	// cost to create this is high so is better create it here
	// instead calling Util
	thread_local std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_real_distribution<double> dist(0, 1);

	std::for_each(g->begin(), g->end(),
			[&generator, &dist, context](Individual * i) {

				GenotypeBit * gn = i->get_genotype();
				for (int i = 0; i < gn->size(); i++) {
					if ( dist(generator) <= context->mutation_rate ) {
						gn->set(i, !gn->at(i));
					}
				}
			});

};

const std::function<void(IContainer*)> basic = [](IContainer* g) {
	Environment* context = Environment::instance();

	std::for_each(g->begin(), g->end(),
			[context](Individual * i) {

				if (Util::should_be_changed(context->mutation_prop)) {
					GenotypeBit * gn = i->get_genotype();
					int i = Util::rand(gn->size());
					gn->set(i, !gn->at(i));
				}});
};

}

#endif /* OPERATORS_MUTATIONCOLLECTION_H_ */
