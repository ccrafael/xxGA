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

#define synchronized(M)  for(std::unique_lock<std::mutex> M##_lock = M; M##_lock; M##_lock.setUnlock())

#include <condition_variable>
#include <mutex>
#include <functional>
#include "log4cxx/logger.h"
#include "ga.h"
#include "Population.h"
#include "Problem.h"
#include "OperatorFactory.h"
#include "Config.h"
#include "Individual.h"
#include "Output.h"

using namespace std;

/*
 * An island has a GA and also a neighborhood of islands where individuals of the population can emigrate.
 */
class Island: public GA {
	vector<Island *> neighborhood;

	static int count;
	static std::mutex sync_mutex;
	static std::condition_variable sync;

	int num_islands;
	int id;
	std::function<IContainer* (Population*)> emigrationSelection;
	std::function<IContainer* (Population*)> immigrationSelection;

	static log4cxx::LoggerPtr logger;

	int maxGenerations;
	int num_migration = 0;

	static constexpr const char* MAX_GENERATIONS_PARAM = "NumberGenerations";
	static constexpr const char * PARAM_RUN_FOR_N_GENERATIONS =
			"MigrationEveryGenerations";


	/*
	 * Method to synchronize all threads before the migration process starts and after the migration process.
	 */
	static void migration_sync(int id, int num_islands);


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

	/*
	 * Select individuals for the immigration process.
	 *
	 * @return  A set of individuals, those individuals are removed from the population.
	 */
	IContainer * getInmigrants();

	bool ready();

public:
	Island(int id, Problem * problem, OperatorFactory * operatorFactory,
			Config * config, Output * output, int num_islands);

	virtual ~Island();

	/*
	 * set the neighbors islands.
	 */
	void set_neighborhood(vector<Island *> neighborhood);

	/*
	 * Evolve the island population until the end of the algorithm.
	 */
	void evolveIsland();


	void init();

};

#endif /* ISLAND_H_ */
