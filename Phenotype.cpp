
#include "Phenotype.h"
#include "Util.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>

using namespace std;

Phenotype::Phenotype(vector<var*> vars, string function, string s)
{
	Phenotype::vars = vars;
	fparser.AddConstant("pi", 3.1415926535897932);
	fparser.Parse(function, s);
}

/**
 * get a var value inside a genotype
 */
double dec(vector<int> gens, int offset, int bits, double min, double max) {
	int index = Util::b2i(gens, offset, bits);
	double step = (max - min) / pow(2,bits);
	return ( step * index ) + min;
}

/**
 * decode the whole genotype as string
 */
string Phenotype::decodeAsString(Genotype * genotype){
	stringstream out;
	int offset = 0;
	for (unsigned int i = 0; i < Phenotype::vars.size(); i++) {
		out<<"x"<<i<<": " << dec(genotype->getGens(), offset, vars[i]->bits, vars[i]->min, vars[i]->max)<<" ";
		offset += vars[i]->bits;
	}
	return out.str();
}

/**
 * decode the whole genotype as array of int
 */
vector<double> Phenotype::decode(Genotype * genotype){
	stringstream out;
	vector<double> result;
	int offset = 0;
	for (unsigned int i = 0; i < Phenotype::vars.size(); i++) {
		result.push_back( dec(genotype->getGens(), offset, vars[i]->bits, vars[i]->min, vars[i]->max));
		offset += vars[i]->bits;
	}
	return result;
}

Phenotype::~Phenotype() {
}

double Phenotype::evaluate(Genotype * genotype) {
	std::vector<double> d = decode( genotype);
	double values [100];
	for (unsigned int i = 0; i < d.size(); i++) {
		values[i] = d.at(i);
	}
	return fparser.Eval( values );
}

