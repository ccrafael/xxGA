/*
 * OperatorFactory.cpp
 *
 *  Created on: 24 Nov 2017
 *      Author: fali
 */

#include "OperatorFactory.h"

OperatorFactory::OperatorFactory(Config * config) {
	this->config = config;

}

OperatorFactory::~OperatorFactory() {
	// nothing to clean
}



/*
 * Create a new crossover operator.
 * @return A new cross overoperator.
 */
std::function<IContainer* (IContainer*)> OperatorFactory::createCrossoverOperator() {
	return crossover::empty;
}

/*
 * Create a new mutation operator.
 * @return A new mutation operator.
 */
std::function<void (IContainer*)> OperatorFactory::createMutationOperator() {
	return mutation::empty;
}

/*
 * Create a new selection operator.
 * @return A new selection operator.
 */
std::function<IContainer* (IContainer*)> OperatorFactory::createParentSelectionOperator() {
	return selection::empty;
}

/*
 * Create a new replacement selection operator.
 * @return A new replacement selection operator.
 */
std::function<IContainer* (IContainer*, IContainer*)> OperatorFactory::createReplacementSelectionOperator() {
	return replacement::empty;
}

/*
 * Create a new Emmigration selection operator.
 * @return A new migration selection operator.
 */
std::function<IContainer* (IContainer*)> OperatorFactory::createEmigrationSelectionOperator() {
	return selection::empty;
}

/*
 * Create a new Immigration selection operator.
 * @return A new migration selection operator.
 */
std::function<IContainer* (IContainer*)> OperatorFactory::createImmigrationSelectionOperator() {
	return selection::empty;
}
