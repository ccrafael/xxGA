/*
 * Environment.h
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */
#ifndef ENVIRONMENT_H_
#define ENVIRONMENT_H_


//#include "Individual.h"
#include "Population.h"
#include "Phenotype.h"

using namespace std;
class Environment {
	Phenotype * ph;
public:
	Population * population;



public:
	Environment(Phenotype* ph);
	virtual ~Environment();

	IContainer recombine(IContainer);
	void mutate(IContainer);
	void evaluate(IContainer);

	friend ostream& operator<< (ostream& os, Environment en) ;

private:
	IContainer defaultCrossover( IContainer parents );
};

#endif /* ENVIRONMENT_H_ */
