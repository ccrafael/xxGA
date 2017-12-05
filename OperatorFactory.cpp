/*
 * OperatorFactory.cpp
 *
 *  Created on: 24 Nov 2017
 *      Author: fali
 */

#include "OperatorFactory.h"

OperatorFactory::OperatorFactory() {
	// TODO Auto-generated constructor stub

}

OperatorFactory::~OperatorFactory() {
	// TODO Auto-generated destructor stub
}



/*
 * Create a new crossover operator.
 * @return A new cross overoperator.
 */
Operator * OperatorFactory::createCrossoverOperator(Config * config) {
	return new EmptyOperator();
}

/*
 * Create a new mutation operator.
 * @return A new mutation operator.
 */
Operator * OperatorFactory::createMutationOperator(Config * config) {
	return new EmptyOperator();
}

/*
 * Create a new selection operator.
 * @return A new selection operator.
 */
Operator * OperatorFactory::createParentSelectionOperator(Config * config) {
	return new EmptyOperator();
}

/*
 * Create a new replacement selection operator.
 * @return A new replacement selection operator.
 */
Operator * OperatorFactory::createReplacementSelectionOperator(Config * config) {
	return new EmptyOperator();
}

/*
 * Create a new Emmigration selection operator.
 * @return A new migration selection operator.
 */
Operator * OperatorFactory::createEmigrationSelectionOperator(Config * config) {
	return new EmptyOperator();
}

/*
 * Create a new Immigration selection operator.
 * @return A new migration selection operator.
 */
Operator * OperatorFactory::createImmigrationSelectionOperator(Config * config) {
	return new EmptyOperator();
}
