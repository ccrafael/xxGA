/*
 * GeneticAlgorithm.cpp
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */
#include "ga.h"

log4cxx::LoggerPtr GA::logger(log4cxx::Logger::getLogger("ga"));

GA::GA(Problem * problem, OperatorFactory * operatorFactory, Config * config,
		Output * output) {
	LOG4CXX_DEBUG(logger, "Creating a new GA.");

	this->problem = problem;
	this->generation = 0;
	this->config = config;
	this->output = output;
	this->population = nullptr;

	// Build the operators
	this->parentSelection = operatorFactory->createParentSelectionOperator();
	this->replacementSelection =
			operatorFactory->createReplacementSelectionOperator();
	this->mutation = operatorFactory->createMutationOperator();
	this->crossover = operatorFactory->createCrossoverOperator();

	this->evaluation = operatorFactory->createEvaluationOperator();
}

GA::~GA() {
}

void GA::init() {
	LOG4CXX_DEBUG(logger, "Initializing population of the  GA.");

	int num_individuals = config->getInt(POPULATION_SIZE_PARAM);
	int num_genes = config->getInt(NUMBER_OF_GENES_PARAM);
	IContainer * individuals = new IContainer();

	for (int i = 0; i < num_individuals; i++) {
		individuals->push_back(new Individual(num_genes, 0));
	}

	// evaluate the individuals using the evaluation operator.
	// Evaluation TODO how to add openCL here?
	this->evaluation(problem, individuals);

	population = new Population();
	population->add(individuals);

	// free memory
	delete individuals;
}


void GA::evolve(int generations) {

	if (population == nullptr) {
		throw invalid_argument("GA not initialized yet.");
	}

	LOG4CXX_TRACE(logger, "Running for "<<generations<<".");

	for (int i = 0; i < generations; i++) {

		// Selection
		IContainer * parents = this->parentSelection(population);

		// Crossover
		IContainer * offspring = this->crossover(parents, generation);

		// Mutation
		this->mutation(offspring);

		// Evaluation TODO how to add openCL here?
		this->evaluation(problem, offspring);

		// Replacement selection
		IContainer * notSurvivors = this->replacementSelection(population,
				offspring);

		// remove the memory consumed by the removed individuals
		this->getPopulation()->remove(notSurvivors);

		// print the current status
		this->output->print_generation(generation, population);

		// clean temporary containers
		delete parents;
		delete offspring;

		while (notSurvivors->size() > 0) {
			Individual * i = notSurvivors->back();
			notSurvivors->pop_back();
			delete i;
		}
		delete notSurvivors;

		this->generation++;

		LOG4CXX_TRACE(logger,
				"Generation: "<<this->generation<<". Best: " << population->best());
	}
	LOG4CXX_DEBUG(logger,
					"Generation: "<<this->generation<<". Best: " << population->best());
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
