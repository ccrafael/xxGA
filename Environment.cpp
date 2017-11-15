/*
 * Environment.cpp
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */

#include "Environment.h"
#include "Population.h"
#include "ga.h"

using namespace std;

Environment::Environment(Phenotype* ph) {
	Environment::ph = ph;
	Environment::population = new Population(GA::IndividualsNumber);
}

Environment::~Environment() {
	delete Environment::population;
}

void Environment::evaluate(IContainer individual) {
	for (unsigned int i = 0; i < individual.size(); i++) {
		if (!individual.at(i)->evaluated()) {
			double fitness = Environment::ph->evaluate(individual.at(i)->getGenotype());
			individual.at(i)->evaluate( fitness );
		}
	}
}

void Environment::mutate(IContainer individual) {
	for (unsigned int i = 0; i < individual.size(); i++) {
		individual.at(i)->mutate(GA::MutationRate);
	}
}

IContainer Environment::recombine(IContainer parents) {
	switch (GA::CrossoverType) {
		case 1: {
			return defaultCrossover(parents);
		}
		default: {
			return defaultCrossover(parents);
		}
	}
}

IContainer Environment::defaultCrossover(IContainer parents) {
	IContainer newGeneration;
	Individual * id = parents.at(0)->crossover( parents.at(1) );
	newGeneration.push_back(id);
	return newGeneration;
}

ostream& operator<< (ostream& os, Environment en) {
	for (unsigned int i =0; i < en.population->getIndividuals().size(); i++) {
		os<<i<<":" << en.ph->decodeAsString(en.population->getIndividuals().at(i)->getGenotype())<<endl;
	}
	return os;
}

