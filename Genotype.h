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

class Genotype {
	vector<int> genotype;
public:
	Genotype(int size);
	virtual ~Genotype();
	Genotype(vector<int> genotype);

	void mutate(double rate);
	void recombination(Genotype*  genotype);

	vector<int> getGens();

	int getSize();
	friend ostream & operator<< (ostream& os,Genotype g);
	friend ostream & operator<< (ostream& os,Genotype * g);
};

#endif /* GENOTYPE_H_ */
