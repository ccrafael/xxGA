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
#include "../GenotypeBit.h"

using namespace std;


class BagProblem: public Problem {
	static log4cxx::LoggerPtr plogger;
	double max_size;
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

	double evaluate(Individual * individual);
	string decode(Individual * individual);

};
#endif /* BAGPROBLEM_H_ */
