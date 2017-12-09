/*
 * Individual.cpp
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */

#include "Individual.h"

Individual::Individual(int genotype_size, int birth) {
	Individual::genotype = new GenotypeBit(genotype_size);
	this->eval = false;
	this->_fitness = 0;
	this->gen_birth = birth;
}

Individual::Individual(GenotypeBit * genotype, int birth){
	Individual::genotype = new GenotypeBit( genotype );
	eval = false;
	this->_fitness = 0;
	this->gen_birth = birth;
}

Individual::~Individual() {
	delete genotype;
}

double Individual::fitness(){
	return _fitness;
}

void Individual::fitness(double value) {
	this->_fitness = value;
}


GenotypeBit * Individual::get_genotype() {
	return Individual::genotype;
}

bool Individual::isEvaluated() {
	return Individual::eval;
}

void Individual::setEvaluated(bool evaluated) {
	this->eval = evaluated;
}


ostream& operator<< (ostream& os, Individual * in ) {
	os << in->get_genotype();
	if (in->isEvaluated())
		os << " - fitness: "<< in->fitness();
	else
		os << " ind not evaluated";
	return os;
}

int operator< (const Individual& id1, const Individual& id2) {
	return id1._fitness < id2._fitness;
}

int Individual::birth() {
	return gen_birth;
}



