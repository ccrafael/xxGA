/*
 * FunctionProblem.h
 *
 *  Created on: 26 Nov 2017
 *      Author: fali
 */

#ifndef BAGPROBLEM_H_
#define BAGPROBLEM_H_

#include <log4cxx/logger.h>
#include <string>
#include <vector>

#include "../Problem.h"

using namespace std;


class BagProblem: public Problem {
	static log4cxx::LoggerPtr plogger;
	double max_size;
	int numgenes;
	vector<double> bins;
public:

	/**
	 * Constructor.
	 * @param filename A string with the file path that contains the properties to load.
	 */
	BagProblem(Config * config);

	/**
	 * Destructor.
	 */
	virtual ~BagProblem();

	double evaluate(Individual * individual) override;
	string decode(Individual * individual) override;
	Individual * create_new_individual(int birth) override;

};
#endif /* BAGPROBLEM_H_ */
