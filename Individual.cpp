/*
 * Individual.cpp
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */

#include "Individual.h"

Individual::Individual(int genotype_size) {
	Individual::genotype = new Genotype(genotype_size);
	eval = false;
}

Individual::Individual(Genotype * genotype){
	Individual::genotype = new Genotype( genotype->getGens() );
	eval = false;
}

Individual::~Individual() {
	delete genotype;
}

Individual * Individual::crossover(Individual * individual) {

	Individual * new_individual = new Individual(Individual::genotype);

	new_individual->getGenotype()->recombination(individual->getGenotype());
	return  new_individual;
}

double Individual::getFitness(){
	return fitness;
}

void Individual::evaluate(double fitness) {
	Individual::fitness = fitness;
	Individual::eval = true;
}

Genotype * Individual::getGenotype() {
	return Individual::genotype;
}

bool Individual::evaluated() {
	return Individual::eval;
}

void Individual::mutate(double rate) {
	Individual::genotype->mutate(rate);
}

ostream& operator<< (ostream& os, Individual * in ) {
	os << in->getGenotype();
	if (in->evaluated())
		os << " - fitness: "<< in->getFitness();
	else
		os << " ind not evaluated";
	return os;
}

int operator< (const Individual& id1, const Individual& id2) {
	return id1.fitness < id2.fitness;
}


