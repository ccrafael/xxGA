/*
 * GeneticAlgorithm.h
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */
#ifndef GENETICALGORITHM_H_
#define GENETICALGORITHM_H_

#include "log4cxx/logger.h"
#include "sys.h"
#include "OperatorFactory.h"
#include "Config.h"
#include "Operator.h"
#include "Population.h"

using namespace std;

/**
 * A GA it is the core of the evolution.
 */
class GA {
	static log4cxx::LoggerPtr logger;

	Operator * parentSelection;
	Operator * replacementSelection;
	Operator * mutation;
	Operator * crossover;

protected:
	Population * population;
	Config * config;
	Problem * problem;

	int generation;

	static constexpr const char* POPULATION_SIZE_PARAM = "population_size";
public:
	GA(Problem * problem, OperatorFactory * operatorFactory, Config * config);
	virtual ~GA();

	/*
	 * Run the evolution process generations times.
	 * @param generations The number of generations that the algorithm will be runned.
	 */
	void evolve(int generations);

	/*
	 * Get a reference to the current population.
	 * @return A reference to the current population.
	 */
	Population * getPopulation();

};

#endif /* GENETICALGORITHM_H_ */
