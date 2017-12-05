/*
 * Environment.cpp
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 *
 */
#include "Island.h"

using namespace std;

log4cxx::LoggerPtr Island::logger(log4cxx::Logger::getLogger("main"));

/**
 * Constructor.
 * @param
 */
Island::Island(Problem * problem, OperatorFactory * operatorFactory, Config * config,
		 vector<Island *> neighborhood): GA(problem, operatorFactory, config) {

	LOG4CXX_DEBUG(logger, "Creating a new Island.");

	this->maxGenerations = config->getInt("max_generations");
	this->neighborhood = neighborhood;
	this->emigrationSelection = operatorFactory->createEmigrationSelectionOperator(config);
	this->immigrationSelection = operatorFactory->createImmigrationSelectionOperator(config);
}

Island::~Island() {
	delete this->emigrationSelection;
}

void Island::evolveIsland() {

	// evolve the population until the stop condition.
	while (!this->isEnd()) {
		// evolve the island for n generations
		evolve(config->getInt(PARAM_RUN_FOR_N_GENERATIONS));

		// emigrate population between neighbors
		emigration();
	}
}

void Island::emigration() {
	vector<Island*>::iterator it = Island::neighborhood.begin();
	while (it != Island::neighborhood.end()) {

		Population * otherPopulation = (*it)->getPopulation();
		Population * myPopulation = this->getPopulation();

		IContainer * immigrants = this->emigrationSelection->operate(otherPopulation->getIndividuals());
		IContainer * emigrants = this->emigrationSelection->operate(myPopulation->getIndividuals());

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

Population * Island::getPopulation() {
	return GA::getPopulation();
}

bool Island::isEnd() {
	return this->maxGenerations < GA::generation;
}

ostream& operator<< (ostream& os, Island en) {
	Population * population = en.getPopulation();
	for (unsigned int i =0; i < population->getIndividuals()->size(); i++) {
		os<<i<<":" << population->getIndividuals()->at(i)<<endl;
	}

	return os;
}

