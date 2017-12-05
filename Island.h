/*
 * Island.h
 *
 *
 *  Created on: 24-nov-2017
 *      Author: rafael
 *
 */
#ifndef ISLAND_H_
#define ISLAND_H_

#include "log4cxx/logger.h"
#include "ga.h"
#include "Population.h"
#include "Problem.h"
#include "OperatorFactory.h"
#include "Config.h"

using namespace std;

/*
 * An island has a GA and also a neighborhood of islands where individuals of the population can emigrate.
 */
class Island: public GA {
	vector<Island *> neighborhood;

	Operator * emigrationSelection;
	Operator * immigrationSelection;

	static log4cxx::LoggerPtr logger;

	static constexpr const char * PARAM_RUN_FOR_N_GENERATIONS =
			"run_for_n_generations";

	int maxGenerations;

	/*
	 * TODO the stop condition should be provided from outside by the problem.
	 *
	 * Determine if the algorithm has finished.
	 * @return True if the algorithm has reached its end condition, false otherwise.
	 */
	bool isEnd();
	void emigration();


	/*
	 * Select individuals for the emigration process.
	 *
	 * @return  A set of individuals, those individuals are removed from the population.
	 */
	IContainer * getEmigrants();


public:
	Island(Problem * problem, OperatorFactory * operatorFactory,
			Config * config, vector<Island *> neighborghood);

	virtual ~Island();

	/*
	 * Evolve the island population until the end of the algorithm.
	 */
	void evolveIsland();

	/*
	 * Get the current population of the island.
	 * @return The population of the island.
	 */
	Population * getPopulation();

	/*
	 * Select individuals for the immigration process.
	 *
	 * @return  A set of individuals, those individuals are removed from the population.
	 */
	IContainer * getInmigrants();

};

#endif /* ISLAND_H_ */
