/*
 * Individual.h
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */
#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_

#include <functional>
#include "Genotype.h"

using namespace std;

class Individual {
	Genotype * genotype;

	double _fitness;
	int gen_birth;
	bool eval;
public:


	/**
	 * create an individual with a suplied genotype.
	 */
	Individual(Genotype * genotype, int birth);

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
	Genotype * get_genotype();


	bool isEvaluated();
	void setEvaluated(bool isEvaluated);


	friend ostream& operator<< (ostream& os, Individual * in );
	friend ostream& operator<< (ostream& os, Individual in );

	int birth();

};


#endif /* INDIVIDUAL_H_ */
