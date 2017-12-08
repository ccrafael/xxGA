/*
 * OperatorFactory.h
 *
 *  Created on: 24 Nov 2017
 *      Author: fali
 */

#ifndef OPERATORFACTORY_H_
#define OPERATORFACTORY_H_

#include <functional>
#include "Config.h"
#include "IContainer.h"
#include "operators/CrossoverCollection.h"
#include "operators/MutationCollection.h"
#include "operators/ReplacementCollection.h"
#include "operators/SelectionCollection.h"

/*
 * A factory used to build the operators that will be used later to apply the evolution
 * process. Each method return a lambda that represent a evolution operator.
 * Take special care that some operator modify the internal collection of objects like the
 * mutation operators. Other operators just return a collection of pointers but that
 * container needs to be deleted by the external user. Works this way in order to save
 * cpu clock cycles.
 */
class OperatorFactory {
	Config * config;

public:
	OperatorFactory(Config * config);
	virtual ~OperatorFactory();

	/*
	 * Create a new crossover operator.
	 * @return A new cross overoperator.
	 */
	std::function<IContainer* (IContainer*)> createCrossoverOperator();

	/*
	 * Create a new mutation operator. This method must update the individuals that are
	 * passed as argument.
	 *
	 * @return A new mutation operator. The function returned has the signature:
	 *      param1: The set of individuals to mutate.
	 */
	std::function<void (IContainer*)> createMutationOperator();

	/*
	 * Create a new selection operator. The function must return a new structure with
	 * the set of selected individuals. The returned structure must be removed when it
	 * is not necessary.
	 *
	 * @return A new selection operator. The function returned has the signature:
	 *        param1 population The population.
	 *        return A new set with the individuals selected.
	 */
	std::function<IContainer* (IContainer*)> createParentSelectionOperator();

	/*
	 * Create a new replacement selection operator. This method return a set of individuals
	 * to be removed from the population. The method must not change the
	 * original structures.
	 * The returned structure should be removed when it is  not necessary.
	 *
	 * @return A new selection operator. The function returned has the signature:
	 *      param1 populaton The population.
	 *      param2 offspring The offspring.
	 *      return A new replacement selection operator.
	 *
	 *
	 */
	std::function<IContainer* (IContainer*, IContainer*)> createReplacementSelectionOperator();

	/*
	 * Create a new emigration selection operator.
	 * @return A new migration selection operator.
	 */
	std::function<IContainer* (IContainer*)> createEmigrationSelectionOperator();

	/*
	 * Create a new immigration selection operator.
	 * @return A new migration selection operator.
	 */
	std::function<IContainer* (IContainer*)> createImmigrationSelectionOperator();

};

#endif /* OPERATORFACTORY_H_ */