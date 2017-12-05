/*
 * OperatorFactory.h
 *
 *  Created on: 24 Nov 2017
 *      Author: fali
 */

#ifndef OPERATORFACTORY_H_
#define OPERATORFACTORY_H_


#include "Operator.h"
#include "operators/EmptyOperator.h"
#include "Config.h"

/*
 * A factory used to build the operators that will be used later to apply the evolution
 * process.
 */
class OperatorFactory {
public:
	OperatorFactory();
	virtual ~OperatorFactory();

	/*
	 * Create a new crossover operator.
	 * @return A new cross overoperator.
	 */
	Operator * createCrossoverOperator(Config * config);

	/*
	 * Create a new mutation operator.
	 * @return A new mutation operator.
	 */
	Operator * createMutationOperator(Config * config);

	/*
	 * Create a new selection operator.
	 * @return A new selection operator.
	 */
	Operator * createParentSelectionOperator(Config * config);

	/*
	 * Create a new replacement selection operator.
	 * @return A new replacement selection operator.
	 */
	Operator * createReplacementSelectionOperator(Config * config);

	/*
	 * Create a new emigration selection operator.
	 * @return A new migration selection operator.
	 */
	Operator * createEmigrationSelectionOperator(Config * config);

	/*
	 * Create a new immigration selection operator.
	 * @return A new migration selection operator.
	 */
	Operator * createImmigrationSelectionOperator(Config * config);

};

#endif /* OPERATORFACTORY_H_ */
