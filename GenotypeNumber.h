/*
 * Genotype.h
 *
 *  Created on: 27-nov-2008
 *      Author: rafael
 */
#ifndef GENOTYPE_H_
#define GENOTYPE_H_

#include <vector>
#include <iostream>
#include <sstream>
#include <cstring>


using namespace std;

class GenotypeNumber {
	vector<double> genotype;
public:
	/*
	 * Create an empty genotype.
	 */
	GenotypeNumber();

	/*
	 * Create a random genotype.
	 */
	GenotypeNumber(int size);

	void set(int index, double value);
	void push_back(double value);

	double at(int i);

	int size();

	friend ostream & operator<< (ostream& os,GenotypeNumber g);
	friend ostream & operator<< (ostream& os,GenotypeNumber * g);

	virtual ~GenotypeNumber();
};

#endif /* GENOTYPE_H_ */
