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
public:
	/**
	 * create an idividual with a random genotype
	 */
	Individual(int genotype_size);

	/**
	 * create an individual with a suplied genotype
	 */
	Individual(Genotype * genotype);

	virtual ~Individual();

	/**
	 * get individual's fitness
	 */
	double getFitness();

	/**
	 *
	 */
	void evaluate(double fitness);


	/**
	 * get individual's genotype
	 */
	Genotype * getGenotype();

	/**
	 * create a new individual crossingover with another one
	 */
	Individual * crossover(Individual * individual) ;

	bool evaluated();

	void mutate(double rate);

	friend ostream& operator<< (ostream& os, Individual * in );
	friend ostream& operator<< (ostream& os, Individual in );

	friend int operator< (const Individual& id1, const Individual& id2);
};

typedef vector<Individual*> IContainer;

#endif /* INDIVIDUAL_H_ */
