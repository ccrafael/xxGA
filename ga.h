/*
 * GeneticAlgorithm.h
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */
#ifndef GENETICALGORITHM_H_
#define GENETICALGORITHM_H_

#include <functional>
#include <chrono>
#include <stdexcept>
#include "log4cxx/logger.h"
#include "OperatorFactory.h"
#include "Config.h"
#include "Population.h"
#include "Individual.h"
#include "Genotype.h"
#include "Output.h"

using namespace std;

/**
 * A GA it is the core of the evolution.
 */
class GA {
	static log4cxx::LoggerPtr logger;

	std::function<IContainer* (Population*, unsigned int)> parentSelection;
	std::function<IContainer* (IContainer*, int)> crossover;
	std::function<void(IContainer*)> mutation;
	std::function<IContainer* (Population*, IContainer*)> replacementSelection;
	std::function<void(Problem*, IContainer*)> evaluation;
	std::function<Individual * (Problem*, int birth)> individual_creator;

	std::chrono::time_point<std::chrono::steady_clock> t0;
	std::chrono::time_point<std::chrono::steady_clock> t1;

	int num_parents;
protected:
	Population * population;
	Config * config;
	Problem * problem;
	Output * output;

	int generation;

public:
	GA(Problem * problem, OperatorFactory * operatorFactory, Config * config,
			Output * output);
	virtual ~GA();

	/*
	 * Run the evolution process generations times.
	 * @param generations The number of generations that the algorithm will be runned.
	 */
	void evolve(int generations);

	/*
	 * Init the population.
	 */
	void init();


	/*
	 * Get a reference to the current population.
	 * @return A reference to the current population.
	 */
	Population * getPopulation();

	Individual * best();
	Individual * worst();

	double mean_fitness();
	double total_fitness();
	double stdev_fitness();

	static constexpr const char* NUMBER_OF_GENES_PARAM = "NumberGenes";
	static constexpr const char* POPULATION_SIZE_PARAM = "NumberIndividuals";
};

#endif /* GENETICALGORITHM_H_ */
