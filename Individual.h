/*
 * Individual.h
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */
#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_

#include "Genotype.h"
using namespace std;

class Individual {
	Genotype * genotype;
	double fitness;
	bool eval;
	bool offspring;
public:
	/**
	 * create an idividual with a random genotype
	 */
	Individual(int genotypeSize);

	/**
	 * create an individual with a suplied genotype
	 */
	Individual(Genotype * genotype);

	virtual ~Individual();

	/**
	 * get individual's fitness
	 */
	double getFitness();

	/*
	 * Set the individual fitness.
	 */
	void setFitness(double fitness);


	/**
	 * get individual's genotype
	 */
	Genotype * getGenotype();


	bool isEvaluated();
	void setEvaluated(bool isEvaluated);

	bool isOffspring();
	void setOffspring(bool isOffspring);


	friend ostream& operator<< (ostream& os, Individual * in );
	friend ostream& operator<< (ostream& os, Individual in );

	friend int operator< (const Individual& id1, const Individual& id2);
};


#endif /* INDIVIDUAL_H_ */
