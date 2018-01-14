/*
 * Image.h
 *
 *  Created on: 12 January 2018
 *      Author: fali
 */

#ifndef PARSEFUNCTIONPROBLEM_H_
#define PARSEFUNCTIONPROBLEM_H_

#include <log4cxx/logger.h>
#include <string>
#include <vector>

#include "../Problem.h"
#include "../Config.h"
#include "../exception/ProblemException.h"
#include "../Individual.h"
#include "../Util.h"
#include "../GenotypeNumber.h"


using namespace std;


class ImageProblem: public Problem {
	static log4cxx::LoggerPtr plogger;

	int numgenes;
	int rows;
	int cols;
	short  ** image;

public:

	/**
	 * Constructor.
	 * @param filename A string with the file path that contains the properties to load.
	 */
	ImageProblem(Config * config);

	/**
	 * Destructor.
	 */
	virtual ~ImageProblem();

	double evaluate(Individual * individual) override;
	string decode(Individual * individual) override;
	Individual * create_new_individual(int birth) override;

};
#endif /* PARSEFUNCTIONPROBLEM_H_ */
