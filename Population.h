/*
 * Population.h
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */
#ifndef POPULATION_H_
#define POPULATION_H_

#include "sys.h"
#include "Individual.h"

using namespace std;

class Population {
	IContainer individuals;
	typedef IContainer::iterator Iter;
	int size;
public:
	Population(int size);
	Population(IContainer population);

	virtual ~Population();

	void init();

	IContainer parentsSelection();

	/**
	 * replace the populatin with a new generation
	 */
	void generationReplacement(IContainer newgeneration);

	/**
	 * get individuals
	 */
	IContainer getIndividuals();

	friend ostream& operator<< (ostream& os, Population * po);
	friend ostream& operator<< (ostream& os, Population po);
private:
	IContainer binaryTournament();
	int menor();
	int selectparent();
	int mayor(vector<int> ind);
};

#endif /* POPULATION_H_ */
