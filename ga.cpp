/*
 * GeneticAlgorithm.cpp
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */
#include "ga.h"

log4cxx::LoggerPtr GA::logger(log4cxx::Logger::getLogger("main"));

GA::GA(Problem * problem, OperatorFactory * operatorFactory, Config * config) {
	LOG4CXX_DEBUG(logger, "Creating a new GA.");

	this->problem = problem;
	this->generation = 0;
	this->config = config;
	this->population = new Population(problem, config->getInt(POPULATION_SIZE_PARAM),
			config->getInt("number_of_genes"));

	// Build the operators
	this->parentSelection = operatorFactory->createParentSelectionOperator(config);
	this->replacementSelection = operatorFactory->createReplacementSelectionOperator(config);
	this->mutation = operatorFactory->createMutationOperator(config);
	this->crossover = operatorFactory->createCrossoverOperator(config);
}

GA::~GA() {
	delete this->population;
	delete this->parentSelection;
	delete this->replacementSelection;
	delete this->mutation;
	delete this->crossover;
}

void GA::evolve(int generations) {

	LOG4CXX_DEBUG(logger, "Running for "<<generations<<".");

	for (int i = 0; i < generations; i++) {

		// Selection
		IContainer * parents = this->parentSelection->operate( population->getIndividuals() );

		// Crossover
		IContainer * offspring = this->crossover->operate( parents );

		// Mutation
		offspring = this->mutation->operate( offspring );

		// Union of the population and new offspring generation
		this->population->add( offspring );

		// Replacement selection
		IContainer * notSurvivors = this->replacementSelection->operate( population->getIndividuals() );

		// Eliminate from the population the individuals that not survived
		population->eliminate( notSurvivors );

		LOG4CXX_DEBUG(logger, "Generation: "<<this->generation<<".");

		this->generation ++;
	}
}


Population * GA::getPopulation() {
	return this->population;
}


