/*
 * ParseFunctionProblem.h
 *
 *  Created on: 26 Nov 2017
 *      Author: fali
 */

#ifndef PARSEFUNCTIONPROBLEM_H_
#define PARSEFUNCTIONPROBLEM_H_

#include <log4cxx/logger.h>
#include <string>
#include <vector>

#include "../Problem.h"
#include "../GenotypeBit.h"
#include "fparser/fparser.hh"


using namespace std;

struct pvariable {
	double min;
	double max;
	int bits;
	double step;
	double count;
};

class ParseFunctionProblem: public Problem {
	static log4cxx::LoggerPtr plogger;

	FunctionParser fparser;
	short num_vars;
	int numgenes;

	// se usa para evaluar la funcion buscando en todo el dominio
	pvariable * configvar;

	/*
	 * Convenience method to get the value of the specific variable from the genotype, the string of bits.
	 * @param gens The vector of genes.
	 * @param offset The offset inside the bitstring.
	 * @param bits The number of bits for that variable.
	 * @param min The min value for that variable.
	 * @param max The max value for that variable.
	 * @param step The precission of the variable it is (max -min)/pow(2, bits). Where bits = NumberGenes/numvars.
	 * @return The double representation of the  variable that start in offset position.
	 */
	double dec(vector<bool> gens, int offset, int bits, double min, double max,
			double step);
	/*
	 * Convenience mehtod to convert from the genotype space to the phenotype space.
	 * @param genotyp The genotype.
	 * @return A vector of doubles where each position is a value in the phenotype space.
	 */
	vector<double> decode(GenotypeBit * genotype);
public:

	/**
	 * Constructor.
	 * @param filename A string with the file path that contains the properties to load.
	 */
	ParseFunctionProblem(Config * config);

	/**
	 * Destructor.
	 */
	virtual ~ParseFunctionProblem();

	double evaluate(Individual * individual) override;
	string decode(Individual * individual);

	Individual * create_new_individual(int birth) override;

};
#endif /* PARSEFUNCTIONPROBLEM_H_ */
