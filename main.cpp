/*
 * test.cpp
 *
 *  Created on: 10-dic-2008
 *      Author: rafael
 */

#include <thread>
#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>

// include log4cxx header files.
#include "log4cxx/logger.h"
#include "log4cxx/propertyconfigurator.h"
#include "log4cxx/helpers/exception.h"

#include "Util.h"
#include "Island.h"
#include "exception/ConfigurationException.h"

#include "problems/BagProblem.h"
#include "Problem.h"
#include "Output.h"
#include "problems/FunctionsProblem.h"
#include "Environment.h"
#include "utest/utests.h"

using namespace log4cxx;
using namespace log4cxx::helpers;

void usage(char** argv) {
	cerr << " Incorrect number of arguments." << endl;
	cout << " Usage: " << argv[0] << " log_config_file xxGA_config_file\n"
			<< argv[0] << " tests " << endl;
}
/*
 * Main method.
 *
 * The program need two command line arguments. The log file path and the algorithm configuration file path.
 *
 */
int main(int argc, char** argv) {

	if (argc == 2) {
		if (string(argv[1]).compare("test") == 0) {
			return tests_main(argc, argv);
		}
	}

	// Simple argument control.
	if (argc != 3) {
		usage(argv);
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
		// TODO redesing this
		Environment * env = Environment::instance();
		env->config = &config;
		env->mutation_rate = config.getDouble("MutationRate");
		env->mutation_prop =  config.getDouble("MutationProbability");

		env->num_parents = config.getInt("NumParents");
		env->tournament_size = config.getInt("TournamentSize");

		env->exchange_probability = config.getDouble("ExchangeProbability");

		env->num_migrants = config.getInt("NumberMigrants");
		env->num_offspring = config.getInt("NumberOffspring");


		LOG4CXX_INFO(logger, "Loading problem and operators.");

		// Load the problem and the operators factory
		/*
		 * TODO create an abstraction of this to easily change the problem.
		 */

		Problem * problem = new FunctionsProblem(&config);
		//Problem *  proble = new BagProblem(&config);

		OperatorFactory operatorFactory(&config);

		Output output(problem, &config);

		// print the header
		output.print_header();

		// start a timer to messure the time consumed by the algorithm
		// the time will include also the algorithm initialization phase
		// note it will not include the config reading and problem instance creation
		output.start();

		LOG4CXX_INFO(logger, "Creating islands.");

		// TODO opencl should be problem specific or algorithm common??
		// TODO move this to a opencl initialization function or samething like that

		int num_islands = config.getInt("NumberIsles");

		std::vector<Island *> islands;

		// vector container stores threads
		std::vector<std::thread> workers;

		for (int i = 0; i < num_islands; i++) {
			Island * island = new Island(i, problem, &operatorFactory, &config,
					&output, num_islands);
			islands.push_back(island);
		}

		LOG4CXX_INFO(logger, "Creating neighborhood.");
		if (num_islands > 1) {
			// this connect all the islands as a ring
			for (int i = 0; i < num_islands; i++) {
				vector<Island*> neihgborhood;
				int left = i > 0 ? i - 1 : num_islands - 1;
				int right = i < num_islands - 1 ? i + 1 : 0;

				neihgborhood.push_back(islands[left]);
				neihgborhood.push_back(islands[right]);
				islands[i]->set_neighborhood(neihgborhood);
			}
		}

		LOG4CXX_INFO(logger, "Launching threads.");
		// Create the num_islands islands and start the evolution into it
		for (int i = 0; i < num_islands; i++) {
			workers.push_back(std::thread([i, islands, logger]() {

				islands[i]->init();

				LOG4CXX_INFO(logger, "Evolving island "<<i<<".");

				// All the interesting things happen here.
					islands[i]->evolveIsland();

				}));

		}

		// wait for the threads
		std::for_each(workers.begin(), workers.end(), [](std::thread &t) {
			t.join();
		});

		// stop the timer
		output.stop();

		// print the conf to identify the experiment later
		output.print_conf();

		LOG4CXX_INFO(logger, "Collecting final results. ");
		// print the final statistics and the results
		// all will be written to the file specified in the configuration file.
		Individual * better = nullptr;
		std::for_each(islands.begin(), islands.end(),
				[&output, &better](Island* i) {
					Population * population = i->getPopulation();
					output.print_final_results(population);
					Individual * best = i->best();
					if (better == nullptr || better->fitness() < best->fitness()) {
						better = best;
					}
				});

		output.print(better);

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
