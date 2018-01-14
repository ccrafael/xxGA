/*
 * Genotype.h
 *
 *  Created on: 27-nov-2008
 *      Author: rafael
 */
#ifndef GenotypeNumber_H_
#define GenotypeNumber_H_

#include <vector>
#include <iostream>
#include "Genotype.h"

using namespace std;

template<class T>
class GenotypeNumber: public Genotype {
	vector<T> genotype;
public:
	/*
	 * Create an empty genotype.
	 */
	GenotypeNumber() {

	}
	;

	void set(int index, T value) {
		genotype[index] = value;
	}
	;

	void push_back(T value) {
		genotype.push_back(value);
	}
	;

	T at(int i) {
		return genotype.at(i);
	}
	;

	int size() {
		return GenotypeNumber::genotype.size();
	}
	;

	void set(GenotypeNumber<T> * g) {
		genotype.clear();
		for (int i = 0; i < g->size(); i++) {
			genotype.push_back( g->at(i));
		}
	}

	friend ostream & operator<<(ostream & os, GenotypeNumber<T> g) {
		for (int i = 0; i < g.size(); i++) {
			os << g.at(i);
		}
		return os;
	}
	;

	friend ostream & operator<<(ostream & os, GenotypeNumber<T> * g) {
		if (g != NULL) {
			for (int i = 0; i < g->size(); i++) {
				os << g->at(i);
			}
		} else {
			os << "null";
		}
		return os;
	}
	;

	virtual ~GenotypeNumber() {
	}
	;
};

#endif /* GenotypeNumber_H_ */
