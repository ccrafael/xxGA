/*
 * Population.cpp
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */
#include "Population.h"

log4cxx::LoggerPtr Population::logger(log4cxx::Logger::getLogger("pupulation"));

Population::Population() {
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
				individuals.erase(r.first);
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
	It it = individuals.begin();
	std::advance(it, n);
	return new IContainer(this->individuals.begin(), it);
}

IContainer * Population::bests(int n) {
	multiset<Individual*>::reverse_iterator it = individuals.rbegin();
	std::advance(it, n);
	return new IContainer(it.base(), individuals.end());
}

double Population::mean_fitness() {
	return total_fitness() / individuals.size();
}

double Population::total_fitness() {
	double sum = 0;
	for (It it = individuals.begin(); it != individuals.end(); ++it) {
		sum += (*it)->fitness();
	}
	return sum;
}

double Population::stdev_fitness(double mean) {
	double sum = 0;
	for (It it = individuals.begin(); it != individuals.end(); ++it) {
		sum += pow((*it)->fitness()- mean, 2);
	}

	return std::sqrt(sum / (individuals.size()-1));
}

int Population::size() {
	return individuals.size();
}

Individual * Population::at(int i) {
	It it = individuals.begin();
	std::advance(it, i);
	return *it;
}
