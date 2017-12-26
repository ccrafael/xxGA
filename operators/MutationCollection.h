/*
 * MutationCollection.h
 *
 *  Created on: 8 Dec 2017
 *      Author: fali
 */

#ifndef OPERATORS_MUTATIONCOLLECTION_H_
#define OPERATORS_MUTATIONCOLLECTION_H_

#include <functional>
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
const std::function<void(IContainer*)> basic = [](IContainer* g) {
	Environment* context = Environment::instance();

	std::for_each(g->begin(), g->end(),
			[context](Individual * i) {
				GenotypeBit * gn = i->get_genotype();
				for (int i = 0; i < gn->size(); i++) {
					if ( Util::should_be_changed(context->mutation_rate) ) {
						gn->set(i, !gn->at(i));
					}
				}
			});

};

}

#endif /* OPERATORS_MUTATIONCOLLECTION_H_ */
