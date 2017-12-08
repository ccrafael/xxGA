/*
 * Genotype.h
 *
 *  Created on: 27-nov-2008
 *      Author: rafael
 */
#ifndef GENOTYPEBIT_H_
#define GENOTYPEBIT_H_

#include <iostream>
#include <vector>


using namespace std;

/*
 * TODO implement a real bit implementation
 * also should be easily interchangable with number genotype
 * may be templates should be used.
 */
class GenotypeBit{
	vector<bool> genotype;
public:
	/*
	 * Create an empty genotype.
	 */
	GenotypeBit();

	/*
	 * Create a random genotype.
	 */
	GenotypeBit(int size);

	GenotypeBit(GenotypeBit * other);

	void set(int index, bool value);
	void push_back(bool value);

	bool at(int i);

	int size();

	friend ostream & operator<< (ostream& os,GenotypeBit g);
	friend ostream & operator<< (ostream& os,GenotypeBit * g);

	virtual ~GenotypeBit();
};

#endif /* GENOTYPEBIT_H_ */
