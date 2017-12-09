/*
 * EvaluationCollection.h
 *
 *  Created on: 8 Dec 2017
 *      Author: fali
 */

#ifndef OPERATORS_EVALUATIONCOLLECTION_H_
#define OPERATORS_EVALUATIONCOLLECTION_H_

#include <functional>
#include "../Problem.h"
#include "../IContainer.h"
#include "../Individual.h"
#include "../GenotypeBit.h"
#include "../exception/OperatorException.h"
#include "../Context.h"

namespace evaluation {

/**
 * Do nothing.
 */
const std::function<void(Problem*, IContainer*)> empty = [](Problem*, IContainer* i) {
	// do what was promised
	};

/**
 * Do nothing.
 */
const std::function<void(Problem*, IContainer*)> basicevaluation = [](Problem* p, IContainer* g) {

	std::for_each(g->begin(), g->end(),
			[p](Individual * i) {
				i->fitness(p->evaluate(i));

			});

};

}

#endif /* OPERATORS_EVALUATIONCOLLECTION_H_ */