/*
 * FunctionsProblem.h
 *
 *  Created on: 26 Nov 2017
 *      Author: fali
 */

#ifndef FUNCTIONSPROBLEM_H_
#define FUNCTIONSPROBLEM_H_

#include <log4cxx/logger.h>
#include <functional>
#include <string>
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>

#include "../Problem.h"
#include "../GenotypeBit.h"

#include "../CLEvaluator.h"

using namespace std;

struct variable {
	double min;
	double max;
	int bits;
	double step;
	double count;
};

class FunctionsProblem: public Problem {
	static log4cxx::LoggerPtr plogger;
	CLEvaluator * clEvaluator;

	short num_vars;

	// se usa para evaluar la funcion buscando en todo el dominio
	variable * configvar;

	std::function<double (vector<double>)> function;

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

	static std::function<double (vector<double>)> sphere;
	static std::function<double (vector<double>)> ackley;
	static std::function<double (vector<double>)> schwefel;
	static std::function<double (vector<double>)> rastrigin;
	static std::function<double (vector<double>)> rosenbrock;


public:

	/*!
	 * Constructor.
	 * @param filename A string with the file path that contains the properties to load.
	 */
	FunctionsProblem(Config * config);

	/*!
	 * Destructor.
	 */
	virtual ~FunctionsProblem();

	double evaluate(Individual * individual) override;
	string decode(Individual * individual) override;

	void clevaluate(IContainer * individuals);

	string kernel_code();

};
#endif /* FUNCTIONSPROBLEM_H_ */
