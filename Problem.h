/*
 * Config.h
 *
 *  Created on: 29-nov-2008
 *      Author: rafael
 */
#ifndef PROBLEM_H_
#define PROBLEM_H_

#include <log4cxx/logger.h>
#include <stdexcept>
#include <string>
#include "Config.h"
#include "Individual.h"
#include "IContainer.h"


using namespace std;

/*
 * Abstract class that represent a problem. Basically it is used to decode, the posible
 * solutions of the problem and also to evaluate those solutions.
 *
 * By default a problem configuratio file is loaded. The configuration file path is
 * configured into the main configuration file with the property ProblemFileConfig
 */
class Problem {
protected:
	static log4cxx::LoggerPtr logger;

	Config * config;
	Config * configProblem;
public:
	/*!
	 * Load the problem configuration into a different  Config object.
	 * Properties of the problem are into different file than properties of the
	 * algorithm.
	 */
	Problem(Config * config);

	/*!
	 * Get the fitness of a individual.
	 * @param individual The individual to evaluate.
	 * @return The fitness associated with the individual.
	 */
	virtual double evaluate(Individual * individual) =  0;

	/*!
	 * Evaluate all individuals at once using opencl.
	 *
	 * This method must return the individuals evaluated.
	 */
	virtual void clevaluate(IContainer * individuals);


	/*!
	 * Decode a individual to show the solution.
	 * @param individual The individual to evaluate.
	 * @return The string representation of that individual in the phenotype plane.
	 */
	virtual string decode(Individual * individual) = 0;

	virtual ~Problem();


private:
	static constexpr const char* PROBLEM_FILE_CONFIG = "ProblemFileConfig";
};


#endif /* PROBLEM_H_ */

