/*
 * Individual.cpp
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */

#include "Individual.h"

Individual::Individual(int genotype_size) {
	Individual::genotype = new Genotype(genotype_size);
	this->eval = false;
	this->offspring = false;
	this->fitness = 0;
}

Individual::Individual(Genotype * genotype){
	Individual::genotype = new Genotype( genotype->getGens() );
	eval = false;
	this->offspring = false;
	this->fitness = 0;
}

Individual::~Individual() {
	delete genotype;
}

double Individual::getFitness(){
	return fitness;
}

void Individual::setFitness(double fitness) {
	this->fitness = fitness;
}


Genotype * Individual::getGenotype() {
	return Individual::genotype;
}

bool Individual::isEvaluated() {
	return Individual::eval;
}

void Individual::setEvaluated(bool evaluated) {
	this->eval = evaluated;
}


ostream& operator<< (ostream& os, Individual * in ) {
	os << in->getGenotype();
	if (in->isEvaluated())
		os << " - fitness: "<< in->getFitness();
	else
		os << " ind not evaluated";
	return os;
}

int operator< (const Individual& id1, const Individual& id2) {
	return id1.fitness < id2.fitness;
}


