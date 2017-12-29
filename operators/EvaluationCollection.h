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
#include "../Environment.h"
#include "log4cxx/logger.h"

namespace evaluation {


static log4cxx::LoggerPtr logger(log4cxx::Logger::getLogger("EvaluationCollection"));

/*!
 * Do nothing.
 */
const std::function<void(Problem*, IContainer*)> empty = [](Problem*, IContainer* i) {
	// do what was promised
	};

/*!
 * \brief Basic evaluation.
 */
const std::function<void(Problem*, IContainer*)> basicevaluation = [](Problem* p, IContainer* g) {

	std::for_each(g->begin(), g->end(),
			[p](Individual * i) {
				i->fitness(p->evaluate(i));
				i->setEvaluated(true);

			});

};



/*!
 * \brief Basic evaluation.
 */
const std::function<void(Problem*, IContainer*)> openclevaluation = [](Problem* p, IContainer* g) {

	p->clevaluate(g);

	if (logger->isDebugEnabled()) {
		// useful to know that the gpu is calculating the same than the cpu
		std::for_each(g->begin(), g->end(),
				[p](Individual * i) {
					cout << p->evaluate(i) << endl;

				});
	}


};

}



#endif /* OPERATORS_EVALUATIONCOLLECTION_H_ */
