/*
 * Genotype.cpp
 *
 *  Created on: 27-nov-2008
 *      Author: rafael
 */
#include "GenotypeNumber.h"
#include "Util.h"

using namespace std;


GenotypeNumber::GenotypeNumber() {
}

GenotypeNumber::GenotypeNumber(int size) {
	for (int i = 0; i < size; i ++) {
		genotype.push_back( Util::rand(2) );
	}
}


void GenotypeNumber::set(int index, double value) {
	genotype[index] = value;
}

void GenotypeNumber::push_back(double value) {
	genotype.push_back(value);
}

double GenotypeNumber::at(int i) {
	return genotype.at(i);
}

GenotypeNumber::~GenotypeNumber() {
}

/*void Genotype::mutate(double rate) {
	for (unsigned int i = 0; i < Genotype::genotype.size(); i++) {
		if ( Util::change(rate) ) {
			genotype[i] = genotype[i]==1?0:1;
		}
	}
}*/




int GenotypeNumber::size() {
	return GenotypeNumber::genotype.size();
}

ostream & operator<< (ostream & os, GenotypeNumber g){
	for (int i = 0; i < g.size(); i++) {
		os << g.at(i) ;
	}
	return os;
}

ostream & operator<< (ostream & os, GenotypeNumber * g) {
	if (g != NULL) {
		for (int i = 0; i < g->size(); i++) {
			os << g->at(i) ;
		}
	} else {
		os << "null";
	}
	return os;
}

