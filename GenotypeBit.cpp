/*
 * Genotype.cpp
 *
 *  Created on: 27-nov-2008
 *      Author: rafael
 */
#include "GenotypeBit.h"

#include "Util.h"

using namespace std;


GenotypeBit::GenotypeBit() {
	//Empty
}

GenotypeBit::GenotypeBit(int size) {
	for (int i = 0; i < size; i ++) {
		genotype.push_back( 0 == Util::rand(2) );
	}
}

GenotypeBit::GenotypeBit(GenotypeBit * other) {
	this->genotype = other->genotype;
}
void GenotypeBit::set(int index, bool value) {
	genotype[index] = value;
}

void GenotypeBit::push_back(bool value) {
	genotype.push_back(value);
}

bool GenotypeBit::at(int i) {
	return genotype.at(i);
}

GenotypeBit::~GenotypeBit() {
}

int GenotypeBit::size() {
	return GenotypeBit::genotype.size();
}

ostream & operator<< (ostream & os, GenotypeBit g){
	for (int i = 0; i < g.size(); i++) {
		os << g.at(i) ;
	}
	return os;
}

ostream & operator<< (ostream & os, GenotypeBit * g) {
	if (g != NULL) {
		for (int i = 0; i < g->size(); i++) {
			os << g->at(i) ;
		}
	} else {
		os << "null";
	}
	return os;
}

