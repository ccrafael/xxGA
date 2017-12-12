/*
 * Population.h
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */
#ifndef POPULATION_H_
#define POPULATION_H_

#include <stdexcept>

#include "log4cxx/logger.h"

#include <memory>
#include <set>
#include <numeric>
#include <cmath>

#include "Problem.h"
#include "Individual.h"
#include "IContainer.h"

using namespace std;

struct ind_comparator {
	int operator()(Individual * a,  Individual * b) {
		return  a->fitness() < b->fitness() ;

	}
};


typedef multiset<Individual*>::iterator It;

/*
 * Represent a population of individuals.
 *
 * The population is stored in a structure ordered by the fitness.
 */
class Population {
	static log4cxx::LoggerPtr logger;

	multiset<Individual*, ind_comparator> individuals; //(ind_comparator);

public:
	/*
	 * Population constructor.
	 */
	Population();
	virtual ~Population();

	IContainer * get_individuals();

	/*
	 * Remove fron the current population the set of individuals passed as
	 * argument. Each object is just removed from the internal structure but its
	 * memory is not liberated.
	 *
	 * @param individuals The set of individuals to remove from population.
	 */
	void remove(IContainer * individuals);

	/*
	 * Add a set of individuals to the current population.
	 * @param individuals The set of individuals to add.
	 */
	void add(IContainer * individuals);

	Individual * best();
	Individual * worst();
	IContainer * worsts(int n);

	int size();

	Individual * at(int i);

	double mean_fitness();
	double total_fitness();
	double stdev_fitness();

};

#endif /* POPULATION_H_ */
