/*
 * FunctionProblem.h
 *
 *  Created on: 26 Nov 2017
 *      Author: fali
 */

#ifndef FUNCTIONPROBLEM_H_
#define FUNCTIONPROBLEM_H_

#include "fparser/fparser.hh"
#include "exception/ProblemException.h"
#include "sys.h"
#include "Util.h"
#include "Problem.h"
#include "Individual.h"

struct variable {
	double min;
	double max;
	int bits;
	double step;
	double count;
};



class FunctionProblem: public Problem {
	map<string, string> properties;
	string filename;
	FunctionParser fparser;
	int num_vars;

	// se usa para evaluar la funcion buscando en todo el dominio
	variable configvar[100];

public:
	/*
	 * Default constructor.
	 */
	FunctionProblem();

	/**
	 * Constructor.
	 * @param filename A string with the file path that contains the properties to load.
	 */
	FunctionProblem(string filename);

	/**
	 * Destructor.
	 */
	virtual ~FunctionProblem();

	double evaluate(Individual * individual);

	/**
	 * Get a property value.
	 * @param pname Property name.
	 */
	string getProperty(string pname);
	int getInt(string pname);
	double getDouble(string pname);
	map<string, string> getPropertiesMap();

};
#endif /* FUNCTIONPROBLEM_H_ */
