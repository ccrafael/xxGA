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
std::mutex Island::guard_mutex;
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
	this->num_emigrants = config->getInt(NUMBER_MIGRANTS_PARAM);

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

		// emigrate population between neighbors
		if ( !this->isEnd() && num_islands > 1) {
			emigration();
		}

		LOG4CXX_DEBUG(logger, " Island: "<< id<<" Generation: "<<generation)
	}
}

void Island::migration_sync(int id, int num_islands) {

	std::unique_lock<std::mutex> lk(sync_mutex);
	if (count < num_islands - 1) {
		LOG4CXX_TRACE(logger, " Island "<<id<<" ----------------> is waiting.");
		count++;

		sync.wait(lk);
	} else {
		LOG4CXX_TRACE(logger, " Island "<<id<<" is nofying all.");
		count = 0;
		sync.notify_all();
	}

}

void Island::emigration() {

	Population * myPopulation = this->getPopulation();

	// extract from my population all the individuals that will leave
	// this can be done concurrently
	IContainer * emigrants = this->emigrationSelection(myPopulation, this->num_emigrants);
	myPopulation->remove(emigrants);

	//synchronization barrier all island wait to start the migration process
	migration_sync(id, num_islands);

	int n = num_emigrants / neighborhood.size();
	int rest = num_emigrants % neighborhood.size();

	unique_lock<mutex> lock(guard_mutex);

	for (vector<Island*>::iterator it = neighborhood.begin();
			it != neighborhood.end(); ++it) {

		Population * otherPopulation = (*it)->getPopulation();

		LOG4CXX_DEBUG(logger,
				"Migration "<< num_migration<< " Island "<<id<<" with num_neighbors ("<<neighborhood.size()
					<<") is migrating with "<<(*it)->id<<" mypop: "<<myPopulation->size()<<" n: "<<n<< " rest: "<<rest<<".");

		if (it == neighborhood.end()-1) {
			n = n + rest;
		}
		IContainer * immigrants = this->immigrationSelection(otherPopulation, n);

		// Exchange the the individuals between the populations
		otherPopulation->remove(immigrants);
		myPopulation->add(immigrants);

		IContainer m(emigrants->begin(), emigrants->begin() + n);
		emigrants->erase(emigrants->begin(), emigrants->begin() + n);
		otherPopulation->add(&m);

		delete immigrants;
	}
	num_migration ++;
	lock.unlock();

	//synchronization barrier all island wait be fully migrated
	migration_sync(id, num_islands);

	delete emigrants;
}

void Island::invalidate() {
	valid = false;
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

