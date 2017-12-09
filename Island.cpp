/*
 * Environment.cpp
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 *
 */
#include "Island.h"

using namespace std;

log4cxx::LoggerPtr Island::logger(log4cxx::Logger::getLogger("island"));

/**
 * Constructor.
 * @param
 */
Island::Island(Problem * problem, OperatorFactory * operatorFactory, Config * config,
		 Output *output, vector<Island *> neighborhood): GA(problem, operatorFactory, config, output) {

	LOG4CXX_INFO(logger, "Creating a new Island.");

	this->maxGenerations = config->getInt(MAX_GENERATIONS_PARAM);
	this->neighborhood = neighborhood;
	this->emigrationSelection = operatorFactory->createEmigrationSelectionOperator();
	this->immigrationSelection = operatorFactory->createImmigrationSelectionOperator();
}

Island::~Island() {
}

/**
 * This is the main loop of the algorithm.
 */
void Island::evolveIsland() {

	int generationsBeforeMigration = config->getInt(PARAM_RUN_FOR_N_GENERATIONS);

	// evolve the population until the stop condition.
	while (!this->isEnd()) {

		// evolve the island for n generations
		evolve(generationsBeforeMigration);

		// emigrate population between neighbors
		emigration();
	}
}


void Island::emigration() {
	LOG4CXX_INFO(logger, "Migration process. There are neighbors: "<<neighborhood.size());

	vector<Island*>::iterator it = Island::neighborhood.begin();
	while (it != Island::neighborhood.end()) {

		Population * otherPopulation = (*it)->getPopulation();
		Population * myPopulation = this->getPopulation();

		IContainer * immigrants = this->emigrationSelection(otherPopulation);
		IContainer * emigrants = this->emigrationSelection(myPopulation);

		// Exchange the the individuals between the populations
		otherPopulation->remove(immigrants);
		myPopulation->remove(emigrants);

		otherPopulation->add(emigrants);
		myPopulation->add(immigrants);
	}
}

IContainer * Island::getInmigrants() {
	return nullptr;
}

IContainer * Island::getEmigrants() {
	return nullptr;

}

bool Island::isEnd() {
	return GA::generation > this->maxGenerations;
}

ostream& operator<< (ostream& os, Island en) {
	Population * population = en.getPopulation();
	IContainer * pop = population->get_individuals();
	for (unsigned int i =0; i < pop->size(); i++) {
		os<<i<<":" << pop->at(i)<<endl;
	}
	// delete container no longer needed
	delete pop;

	return os;
}

