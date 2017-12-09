/*
 * Population.cpp
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */
#include "Population.h"

log4cxx::LoggerPtr Population::logger(log4cxx::Logger::getLogger("pupulation"));

Population::Population(Problem * problem, int populationSize, int genesSize) {
	this->genesSize = genesSize;
	this->problem = problem;

	LOG4CXX_INFO(logger,
			"Creating a new population of size: "<< populationSize<< " and genes size: "<<genesSize);

	// Create a random population
	for (int i = 0; i < populationSize; i++) {
		Individual * individual = new Individual(genesSize, 0);

		double fitness = problem->evaluate(individual);
		individual->fitness(fitness);
		individual->setEvaluated(true);

		this->individuals.insert(individual);
	}

	LOG4CXX_DEBUG(logger, " Population initialized. ");
}

Population::~Population() {
	while (this->individuals.size() > 0) {
		multiset<Individual*>::iterator it = this->individuals.begin();
		this->individuals.erase(it);

		delete *it;
	}
}

IContainer * Population::get_individuals() {
	IContainer * container = new IContainer(individuals.begin(),
			individuals.end());
	return container;
}

void required_not_null(IContainer * group, const char * msg) {
	if (group == nullptr) {
		throw invalid_argument(msg);
	}
}
/*
 * Remove fron the current population the set of individuals passed as
 * argument. Each object is just removed from the internal structure but its
 * memory is not liberated.
 *
 * @param individuals The set of individuals to remove from population.
 */
void Population::remove(IContainer * group) {
	required_not_null(group, "null cant be removed from  population. ");

	// cnat remove using erase because it uses key_comp
	// we just want to remove specific pointers
	std::for_each(group->begin(), group->end(), [this](Individual * i) {

		// fist we need to locate the exact pointer
			pair<It, It> r = individuals.equal_range(i);

			while (*r.first != i && r.first != r.second) {
				r.first ++;
			}

			if (*r.first == i) {
				individuals.erase(individuals.begin());
			}
		});

}
/*
 * The same than remove method but the individuals  memory are liberated.
 * @param individuals The set of individuals to completely remove from population
 * and free its memory.
 */
void Population::eliminate(IContainer * group) {
	required_not_null(group, "null cant be eliminated from population. ");
	std::for_each(group->begin(), group->end(), [this](Individual * i) {
		// fist we need to locate the exact pointer
		pair<It, It> r = individuals.equal_range(i);

		while (*r.first != i && r.first != r.second) {
			r.first ++;
		}

		if (*r.first == i) {
			individuals.erase(individuals.begin());
			delete i;
		}

	});
}
/*
 * Add a set of individuals to the current population. The individuals will be reevaluated
 * when they are added to the population.
 * @param individuals The set of individuals to add.
 */
void Population::add(IContainer * group) {
	required_not_null(group, "null cant be added to population. ");
	std::for_each(group->begin(), group->end(), [this](Individual * i) {
		individuals.insert(i);

	});
}

Individual* Population::best() {
	return (*this->individuals.rbegin());
}

Individual* Population::worst() {
	return (*this->individuals.begin());
}

IContainer * Population::worsts(int n) {
	multiset<Individual*>::iterator it = individuals.begin();
	std::advance(it, n);
	return new IContainer(this->individuals.begin(), it);
}

double Population::mean_fitness() {
	return total_fitness() / individuals.size();
}

double Population::total_fitness() {
	std::vector<double> fitnesses(individuals.size());
	std::transform(individuals.begin(), individuals.end(), fitnesses.begin(),
			[](Individual * i) {return i->fitness();});
	return accumulate(fitnesses.begin(), fitnesses.end(), 0.0);
}

double Population::stdev_fitness() {

	double mean = mean_fitness();
	std::vector<double> diff(individuals.size());

	std::transform(individuals.begin(), individuals.end(), diff.begin(),
			[mean](Individual * x) {return pow(x->fitness()- mean, 2);});

	double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(),
			0.0);
	return std::sqrt(sq_sum / individuals.size());

	return 0.0;
}

int Population::size() {
	return individuals.size();
}

Individual * Population::at(int i) {
	It it = individuals.begin();
	std::advance(it, i);
	return *it;
}
