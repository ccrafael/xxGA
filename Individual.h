/*
 * Individual.h
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */
#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_

#include "GenotypeBit.h"
using namespace std;

class Individual {
	GenotypeBit * genotype;

	double _fitness;
	int gen_birth;
	bool eval;
public:
	/**
	 * create an idividual with a random genotype
	 */
	Individual(int genotypeSize, int birth);

	/**
	 * create an individual with a suplied genotype
	 */
	Individual(GenotypeBit * genotype, int birth);

	virtual ~Individual();

	/**
	 * get individual's fitness
	 */
	double fitness();

	/*
	 * Set the individual fitness.
	 */
	void fitness(double fitness);


	/**
	 * get individual's genotype
	 */
	GenotypeBit * get_genotype();


	bool isEvaluated();
	void setEvaluated(bool isEvaluated);


	friend ostream& operator<< (ostream& os, Individual * in );
	friend ostream& operator<< (ostream& os, Individual in );


	int birth();

};


#endif /* INDIVIDUAL_H_ */
