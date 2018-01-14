/*
 * Individual.cpp
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */

#include "Individual.h"


Individual::Individual(Genotype * genotype, int birth){
	Individual::genotype = genotype;
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


Genotype * Individual::get_genotype() {
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

int Individual::birth() {
	return gen_birth;
}



