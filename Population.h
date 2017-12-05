/*
 * Population.h
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */
#ifndef POPULATION_H_
#define POPULATION_H_

#include "log4cxx/logger.h"

#include <memory>
#include <set>
#include "Problem.h"
#include "Individual.h"
#include "IContainer.h"

using namespace std;

struct ind_comparator {
	bool operator() (const Individual * a, const Individual * b) {
		return a < b;
	}
};

/*
 * Represent a population of individuals.
 *
 * The population is stored in a structure ordered by the fitness.
 */
class Population {
	static log4cxx::LoggerPtr logger;

	multiset<Individual*, ind_comparator> individuals;//(ind_comparator);
	multiset<Individual*>::iterator it;

	Problem * problem;
	int genesSize;
	int size;
public:
	/*
	 * Population constructor. In the constructor a random population of size individuals
	 * is created.
	 * @param The size of the population.
	 * @param The size of the genotype.
	 */
	Population(Problem * problem, int size, int genesSize);
	virtual ~Population();


	IContainer * getIndividuals();

	/*
	 * Remove fron the current population the set of individuals passed as
	 * argument. Each object is just removed from the internal structure but its
	 * memory is not liberated.
	 *
	 * @param individuals The set of individuals to remove from population.
	 */
	void remove(IContainer * individuals);
	/*
	 * The same than remove method but the individuals  memory are liberated.
	 * @param individuals The set of individuals to completely remove from population
	 * and free its memory.
	 */
	void eliminate(IContainer * individuals);
	/*
	 * Add a set of individuals to the current population. The individuals will be reevaluated
	 * when they are added to the population.
	 * @param individuals The set of individuals to add.
	 */
	void add(IContainer * individuals);

	friend ostream& operator<< (ostream& os, Population * po);
	friend ostream& operator<< (ostream& os, Population po);
};

#endif /* POPULATION_H_ */
