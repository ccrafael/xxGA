/*
 * Genotype.h
 *
 *  Created on: 27-nov-2008
 *      Author: rafael
 */
#ifndef GENOTYPE_H_
#define GENOTYPE_H_

#include <iostream>

using namespace std;

/*
 *!brief just an interface
 */
class Genotype {

public:

	virtual int size()=0;
	virtual ~Genotype() {};


};

#endif /* GENOTYPE_H_ */
