/*
 * GeneticAlgorithm.cpp
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */
#include "ga.h"

log4cxx::LoggerPtr GA::logger(log4cxx::Logger::getLogger("ga"));

GA::GA(Problem * problem, OperatorFactory * operatorFactory, Config * config, Output * output) {
	LOG4CXX_INFO(logger, "Creating a new GA.");

	this->problem = problem;
	this->generation = 0;
	this->config = config;
	this->output = output;
	this->population = new Population(problem, config->getInt(POPULATION_SIZE_PARAM),
			config->getInt(NUMBER_OF_GENES_PARAM));

	// Build the operators
	this->parentSelection = operatorFactory->createParentSelectionOperator();
	this->replacementSelection = operatorFactory->createReplacementSelectionOperator();
	this->mutation = operatorFactory->createMutationOperator();
	this->crossover = operatorFactory->createCrossoverOperator();
}

GA::~GA() {
}

void GA::evolve(int generations) {

	LOG4CXX_DEBUG(logger, "Running for "<<generations<<".");

	for (int i = 0; i < generations; i++) {

		LOG4CXX_DEBUG(logger, "Current population.");
		IContainer * current_population = population->get_individuals() ;

		LOG4CXX_DEBUG(logger, "Selection.");
		// Selection
		IContainer * parents = this->parentSelection( current_population );

		LOG4CXX_DEBUG(logger, "Crossover.");
		// Crossover
		IContainer * offspring = this->crossover( parents );

		LOG4CXX_DEBUG(logger, "Crossover.");
		// Mutation
		this->mutation( offspring );

		LOG4CXX_DEBUG(logger, "Replacement.");
		// Replacement selection
		IContainer * notSurvivors = this->replacementSelection(current_population, offspring);

		LOG4CXX_DEBUG(logger, "cleaning.");

		// remove the memory consumed by the removed individuals
		this->getPopulation()->remove( notSurvivors );


		// print the current status
		this->output->print_generation(generation, population );


		// clean temporary containers
		delete parents;
		delete offspring;
		delete notSurvivors;
		delete current_population;

		this->generation ++;

		LOG4CXX_DEBUG(logger, "Generation: "<<this->generation<<" completed.");
	}
}


Population * GA::getPopulation() {
	return this->population;
}

Individual* GA::best() {
	return population->best();
}

Individual* GA::worst() {
	return population->worst();
}

double GA::mean_fitness() {
	return population->mean_fitness();
}

double GA::total_fitness() {
	return population->total_fitness();
}
