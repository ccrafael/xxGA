/*
 * test.cpp
 *
 *  Created on: 10-dic-2008
 *      Author: rafael
 */
#include "Util.h"
#include "Island.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>

// include log4cxx header files.
//TODO create configuration
#include "log4cxx/logger.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/helpers/exception.h"

#include "FunctionProblem.h"
#include "Problem.h"

using namespace log4cxx;
using namespace log4cxx::helpers;



int main( int argc, char** argv ) {

	// Configuring the logger library
	BasicConfigurator::configure();
    LoggerPtr logger(Logger::getLogger("main"));

    LOG4CXX_DEBUG(logger, "Starting app.");

    Config config;
    FunctionProblem problem;
    OperatorFactory operatorFactory;
    vector<Island*> neihgborhood;

    Island island(&problem, &operatorFactory, &config, neihgborhood );

    island.evolve(100);

	return 0;
}
