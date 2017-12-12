/*
 * Environment.cpp
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 *
 */
#include "Island.h"

using namespace std;

// static field definitions
log4cxx::LoggerPtr Island::logger(log4cxx::Logger::getLogger("island"));
int Island::count = 0;
std::mutex Island::sync_mutex;
std::condition_variable Island::sync;

/**
 * Constructor.
 * @param
 */
Island::Island(int id, Problem * problem, OperatorFactory * operatorFactory,
		Config * config, Output *output, int num_islands) :
		GA(problem, operatorFactory, config, output) {

	LOG4CXX_DEBUG(logger, "Creating a new Island with id: "<< id<<".");
	this->id = id;
	this->maxGenerations = config->getInt(MAX_GENERATIONS_PARAM);
	this->emigrationSelection =
			operatorFactory->createEmigrationSelectionOperator();
	this->immigrationSelection =
			operatorFactory->createImmigrationSelectionOperator();
	this->num_islands = num_islands;

}

void Island::set_neighborhood(vector<Island *> neighborhood) {
	this->neighborhood = neighborhood;
}

Island::~Island() {
}

void Island::init() {
	GA::init();
}
/**
 * This is the main loop of the algorithm.
 */
void Island::evolveIsland() {

	int generationsBeforeMigration = config->getInt(
			PARAM_RUN_FOR_N_GENERATIONS);

	// evolve the population until the stop condition.
	while (!this->isEnd()) {

		// evolve the island for n generations
		evolve(generationsBeforeMigration);

		//synchronization barrier all island wait to start the migration process
		migration_sync(id, num_islands);

		// emigrate population between neighbors
		emigration();

		//synchronization barrier all island wait be fully migrated
		migration_sync(id, num_islands);

	}
}

void Island::migration_sync(int id, int num_islands) {

	std::unique_lock<std::mutex> lk(sync_mutex);
	if (count < num_islands-1) {
		LOG4CXX_TRACE(logger, " Island "<<id<<" is waiting.");
		count ++;

		sync.wait(lk);
	} else {
		LOG4CXX_TRACE(logger, " Island "<<id<<" is nofying all.");
		count = 0;
		sync.notify_all();
	}

}

void Island::emigration() {

	std::for_each(neighborhood.begin(), neighborhood.end(),
			[this](Island * other) {
				unique_lock<mutex> lock(sync_mutex);

				LOG4CXX_DEBUG(logger, "Migration "<< num_migration<< " Island "<<id<<" with num_neighbors ("<<neighborhood.size()<<") is migrating with "<<other->id<<".");
				Population * otherPopulation = other->getPopulation();
				Population * myPopulation = this->getPopulation();

				IContainer * immigrants = this->emigrationSelection(otherPopulation);
				IContainer * emigrants = this->emigrationSelection(myPopulation);

				// Exchange the the individuals between the populations
				otherPopulation->remove(immigrants);
				myPopulation->remove(emigrants);

				otherPopulation->add(emigrants);
				myPopulation->add(immigrants);

				delete immigrants;
				delete emigrants;

			});
}


bool Island::isEnd() {
	return GA::generation > this->maxGenerations;
}

ostream& operator<<(ostream& os, Island en) {
	Population * population = en.getPopulation();
	IContainer * pop = population->get_individuals();
	for (unsigned int i = 0; i < pop->size(); i++) {
		os << i << ":" << pop->at(i) << endl;
	}
	// delete container no longer needed
	delete pop;

	return os;
}

