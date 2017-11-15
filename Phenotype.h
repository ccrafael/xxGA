#ifndef PHENOTYPE_H_
#define PHENOTYPE_H_

#include "Genotype.h"
#include "fparser/fparser.hh"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct var {
	double min;
	double max;
	int bits;
//	var::var(){
//		min = 0;
//		max = 0;
//		bits = 0;
//	}
};

class Phenotype {
	vector<var*> vars;
	FunctionParser fparser;
public:
	Phenotype(vector<var*> vars, string function, string s);

	string decodeAsString(Genotype * genotype);
	vector<double> decode(Genotype * genotype);

	double evaluate(Genotype * genotype);

	virtual ~Phenotype();
};

#endif /*PHENOTYPE_H_*/
