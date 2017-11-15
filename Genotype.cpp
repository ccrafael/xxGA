/*
 * Genotype.cpp
 *
 *  Created on: 27-nov-2008
 *      Author: rafael
 */
#include "Genotype.h"
#include "Util.h"

using namespace std;

Genotype::Genotype(int size) {
	for (int i = 0; i < size; i ++) {
		genotype.push_back( Util::rand(2) );
	}
}

Genotype::Genotype(vector<int> genotype) {
	for (unsigned int i = 0; i < genotype.size(); i++) {
		Genotype::genotype.push_back(genotype.at(i));
	}
}

Genotype::~Genotype() {
}

void Genotype::mutate(double rate) {
	for (unsigned int i = 0; i < Genotype::genotype.size(); i++) {
		if ( Util::change(rate) ) {
			genotype[i] = genotype[i]==1?0:1;
		}
	}
}

vector<int> Genotype::getGens() {
	return genotype;
}

void Genotype::recombination(Genotype * genotype) {

	int cross_point = Util::rand(Genotype::genotype.size()-2) +1;
	for (unsigned int i = cross_point; i < Genotype::genotype.size(); i++) {
		Genotype::genotype[i] = genotype->getGens().at(i);
	}

}

int Genotype::getSize() {
	return Genotype::genotype.size();
}

ostream & operator<< (ostream & os, Genotype g){
	for (int i = 0; i < g.getSize(); i++) {
		os << g.getGens().at(i) ;
	}
	return os;
}

ostream & operator<< (ostream & os, Genotype * g) {
	if (g != NULL) {
		for (int i = 0; i < g->getSize(); i++) {
			os << g->getGens().at(i) ;
		}
	} else {
		os << "null";
	}
	return os;
}

