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
	IContainer  individuals;

	LOG4CXX_TRACE(logger, "Create "<<num_individuals<<" of size "<<num_genes<<".");
	for (int i = 0; i < num_individuals; i++) {
		individuals.push_back(new Individual(num_genes, 0));
	}

	LOG4CXX_TRACE(logger, "Ind created.");

	// evaluate the individuals using the evaluation operator.
	// Evaluation TODO how to add openCL here?
	this->evaluation(problem, &individuals);

	population = new Population();
	population->add(&individuals);

	LOG4CXX_DEBUG(logger, "Pop initialized.");
}

void GA::evolve(int generations) {

	if (population == nullptr) {
		throw invalid_argument("GA not initialized yet.");
	}

	LOG4CXX_TRACE(logger, "Running for "<<generations<<".");

	for (int i = 0; i < generations; i++) {

		// Selection
		t0 = chrono::steady_clock::now();
		IContainer * parents = this->parentSelection(population);
		t1 = chrono::steady_clock::now();
		output->selection(chrono::duration<double, milli>(t1 - t0).count());

		// Crossover
		t0 = chrono::steady_clock::now();
		IContainer * offspring = this->crossover(parents, generation);
		t1 = chrono::steady_clock::now();
		output->crossover(chrono::duration<double, milli>(t1 - t0).count());

		// Mutation
		t0 = chrono::steady_clock::now();
		this->mutation(offspring);
		t1 = chrono::steady_clock::now();
		output->mutation(chrono::duration<double, milli>(t1 - t0).count());

		// Evaluation TODO how to add openCL here?
		t0 = chrono::steady_clock::now();
		this->evaluation(problem, offspring);
		t1 = chrono::steady_clock::now();
		output->eval(chrono::duration<double, milli>(t1 - t0).count());

		// Replacement selection
		t0 = chrono::steady_clock::now();
		IContainer * notSurvivors = this->replacementSelection(population,
				offspring);

		// remove the memory consumed by the removed individuals
		this->getPopulation()->remove(notSurvivors);

		t1 = chrono::steady_clock::now();
		output->replacement(chrono::duration<double, milli>(t1 - t0).count());

		// print the current status
		this->output->print_generation(generation, population);

		// clean temporary containers
		delete parents;
		delete offspring;

		for (IContainer::iterator it = notSurvivors->begin();
				it != notSurvivors->end(); ++it) {
			delete (*it);
		}

		delete notSurvivors;

		// end
		this->generation++;

		// is written this way because the macro will add an if
		LOG4CXX_TRACE(logger,
				"Generation: "<<this->generation<<". Best: " << population->best() << " ["<<problem->decode(population->best())<<"]");
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
