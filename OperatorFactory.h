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
#include "operators/EvaluationCollection.h"
#include "log4cxx/logger.h"

/*
 * A factory used to build the operators that will be used later to apply the evolution
 * process. Each method return a lambda that represent a evolution operator.
 * Take special care that some operator modify the internal collection of objects like the
 * mutation operators. Other operators just return a collection of pointers but that
 * container needs to be deleted by the external user. Works this way in order to save
 * cpu clock cycles.
 */
class OperatorFactory {
	static log4cxx::LoggerPtr logger;
	Config * config;

public:
	OperatorFactory(Config * config);
	virtual ~OperatorFactory();

	/*
	 * Create a new crossover operator.
	 * @return A new cross overoperator. The function returned has the signature:
	 *     param1 A set of parents to create offspring.
	 *     param2 The current generation number.
	 *     return The set of the offspring.
	 */
	std::function<IContainer* (IContainer*, int)> createCrossoverOperator();

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
	 *        param2 n The number of individuals to be selected.
	 *        return A new set with the individuals selected.
	 */
	std::function<IContainer* (Population*, unsigned int)> createParentSelectionOperator();

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
	std::function<IContainer* (Population*, IContainer*)> createReplacementSelectionOperator();

	/*!
	 * \brief Create a new emigration selection operator.
	 * @return A new migration selection operator. The function returned has the signature:
	 *    param1 population The poupulation to select from.
	 *    param2 n The number of individuals to select.
	 *    return A new container with the selected individuals.
	 */
	std::function<IContainer* (Population*, unsigned int)> createEmigrationSelectionOperator();

	/*!
	 * \brief Create a new immigration selection operator.
	 *
	 * @return A new immigration selection operator. The function returned has the signature:
	 *    param1 population The poupulation to select from.
	 *    param2 n The number of individuals to select.
	 *    return A new container with the selected individuals.
	 */
	std::function<IContainer* (Population*, unsigned int)> createImmigrationSelectionOperator();


	/*
	 * Create the evaluation operator. This operator must update the fitness of
	 * each individual depending on the problem being solved.
	 *
	 * @return A new evaluation operator. The function returned has the signature:
	 *     param1 The problem instance to solve.
	 *     param2 The set of offspring to evaluate.
	 */
	std::function<void (Problem*, IContainer*)> createEvaluationOperator();


	/*
	 * Create the individual creator operator. This operator must create new individuals.
	 *
	 * @return A new creator operator. The function returned has the signature:
	 *     param1 The problem reference.
	 *     param2 The birth date in generations.
	 *     return A new genotype for a individual.
	 */
	std::function<Individual * (Problem *, int birth)> createIndividualCreator();

};

#endif /* OPERATORFACTORY_H_ */
