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
	for (int i = 0; i < size; i++) {
		genotype.push_back(0 == Util::rand(2));
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

vector<bool> GenotypeBit::grayToBinary() {
	vector<bool> binary = genotype;
	for (unsigned int i = 1; i < genotype.size(); i++) {
		if (!genotype[i]) {
			binary[i] = binary[i-1];
		} else {
			binary[i] = !binary[i-1];
		}

	}
	return binary;
}


void GenotypeBit::binaryToGray() {
	vector<bool> gray = genotype;
	for (unsigned int i = 1; i < genotype.size(); i++) {
		gray[i] = !(genotype[i-1] == genotype[i]);
	}
	genotype = gray;

}

ostream & operator<<(ostream & os, GenotypeBit g) {
	for (int i = 0; i < g.size(); i++) {
		os << g.at(i);
	}
	return os;
}

ostream & operator<<(ostream & os, GenotypeBit * g) {
	if (g != NULL) {
		for (int i = 0; i < g->size(); i++) {
			os << g->at(i);
		}
	} else {
		os << "null";
	}
	return os;
}

