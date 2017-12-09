/*
 * test.cpp
 *
 *  Created on: 10-dic-2008
 *      Author: rafael
 */
#include "Util.h"
#include "Island.h"
#include "exception/ConfigurationException.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>

// include log4cxx header files.
#include "log4cxx/logger.h"
#include "log4cxx/propertyconfigurator.h"
#include "log4cxx/helpers/exception.h"

#include "problems/FunctionProblem.h"
#include "Problem.h"
#include "Output.h"
#include "Context.h"

using namespace log4cxx;
using namespace log4cxx::helpers;

/*
 * Main method.
 *
 * The program need two command line arguments. The log file path and the algorithm configuration file path.
 *
 */
int main(int argc, char** argv) {

	// Simple argument control.
	if (argc != 3) {
		cerr << " Incorrect number of arguments." << endl;
		cout << " Usage: " << argv[0] << " log_config_file xxGA_config_file"
				<< endl;
		return 1;
	}

	// Configuring the logger library
	string logfilename(argv[1]);
	PropertyConfigurator::configure(logfilename);
	LoggerPtr logger(Logger::getLogger("main"));

	try {
		LOG4CXX_INFO(logger, "Starting app.");

		// loading the algorithm configuration
		string filename(argv[2]);

		Config config(filename);

		// save some global vars for fast access of the operators
		Context * context = Context::instance();
		context->config = &config;
		context->mutation_rate = config.getDouble("MutationRate");
		context->num_parents = config.getInt("NumParents");
		context->tournament_size = config.getInt("TournamentSize");
		context->exchange_probability = config.getDouble("ExchangeProbability");
		context->num_migrants = config.getInt("NumberMigrants");

		LOG4CXX_INFO(logger, "Loading problem and operators.");

		// Load the problem and the operators factory
		/*
		 * TODO create an abstraction of this to easily change the problem.
		 */
		FunctionProblem problem(&config);

		OperatorFactory operatorFactory(&config);
		vector<Island*> neihgborhood;

		Output output(&problem, &config);

		// print the header
		output.print_header();

		// start a timer to messure the time consumed by the algorithm
		// the time will include also the algorithm initialization phase
		// note it will not include the config reading and problem instance creation
		output.start();

		LOG4CXX_INFO(logger, "Creating islands.");

		// Create the islands
		Island island(&problem, &operatorFactory, &config, &output,
				neihgborhood);

		LOG4CXX_INFO(logger, "Evolving.");

		// All the interesting things happen here.
		island.evolveIsland();

		// stop the timer
		output.stop();

		// print the final statistics and the results
		// all will be written to the file specified in the configuration file.
		output.print_final_results(island.getPopulation());

	} catch (ConfigurationException &e) {
		LOG4CXX_ERROR(logger, " Error loading configuration: "<<e.getMsg());
		cerr << e.getMsg() << endl;
		return 1;
	} catch (NotFoundException &e) {
		LOG4CXX_ERROR(logger, " Error reading property: "<<e.getMsg());
		return 1;
	} catch (const std::exception& re) {
		LOG4CXX_ERROR(logger, "exception: "<<re.what());
		return 1;
	} catch (...) {
		std::exception_ptr p = std::current_exception();

		LOG4CXX_ERROR(logger,
				" Unknown error: "<<(p ? p.__cxa_exception_type()->name() : "null"));

		return 1;
	}

	LOG4CXX_INFO(logger, "This is the end.");

	return 0;
}
