/*
 * Population.cpp
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */
#include "Population.h"

log4cxx::LoggerPtr Population::logger(log4cxx::Logger::getLogger("main"));

Population::Population(Problem * problem, int populationSize, int genesSize) {
	this->size = populationSize;
	this->genesSize = genesSize;
	this->problem = problem;

	// Create a random population
	for (int i = 0; i < size; i++) {
		Individual * individual = new Individual(genesSize);

		double fitness = problem->evaluate(individual);
		individual->setFitness(fitness);
		individual->setEvaluated(true);

		this->individuals.insert(individual);
	}
}

Population::~Population() {
	while (this->individuals.size() > 0) {
		it = this->individuals.begin();
		this->individuals.erase(it);

		delete *it;
	}
}

IContainer * Population::getIndividuals() {
	return nullptr;
}

/*
 * Remove fron the current population the set of individuals passed as
 * argument. Each object is just removed from the internal structure but its
 * memory is not liberated.
 *
 * @param individuals The set of individuals to remove from population.
 */
void Population::remove(IContainer * individuals) {

}
/*
 * The same than remove method but the individuals  memory are liberated.
 * @param individuals The set of individuals to completely remove from population
 * and free its memory.
 */
void Population::eliminate(IContainer * individuals) {

}
/*
 * Add a set of individuals to the current population. The individuals will be reevaluated
 * when they are added to the population.
 * @param individuals The set of individuals to add.
 */
void Population::add(IContainer * individuals) {

}

ostream& operator<<(ostream& os, Population * po) {
	for (unsigned int i = 0; i < po->getIndividuals()->size(); i++) {
		os << "  " << i << ":" << po->getIndividuals()->at(i) << std::endl;
	}
	return os;
}

ostream& operator<<(ostream& os, Population po) {
	for (unsigned int i = 0; i < po.getIndividuals()->size(); i++) {
		os << "  " << i << ":" << po.getIndividuals()->at(i) << endl;
	}
	return os;
}
