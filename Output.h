/*
 * Output.h
 *
 *  Created on: 6 Dec 2017
 *      Author: fali
 */

#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <ctime>
#include <vector>

#include "Config.h"
#include "Population.h"
#include "Individual.h"

using namespace std;

/*
 * This class print the results to a file.
 *
 * The results can be writen at the end, with the best absolute solution found.
 * or  generation by beneration.
 *
 * Note all threads writes to the same file there fore this class is thread safe.
 */
class Output {
	static log4cxx::LoggerPtr logger;

	Config * config;
	Problem * problem;
	ofstream generation_file;
	ofstream  solution_file;
	int numgen_to_print;
	int execution;

	std::mutex m;
	static constexpr const char* SEPARATOR = ",";

	std::chrono::time_point<std::chrono::steady_clock> t0;
	std::chrono::time_point<std::chrono::steady_clock> t1;

	int eval_count;
	double eval_sum;

	int mutation_count;
	double mutation_sum;

	int selection_count;
	double selection_sum;

	int crossover_count;
	double crossover_sum;

	int replacement_count;
	double replacement_sum;

public:
	Output(Problem * problem, Config * config);
	virtual ~Output();

	/*
	 * print the current status to the file.
	 * A line with this format will be printed to the output file.
	 *
	 * threadid, generation, bestfitness, totalfitness, meanfitness, fitness_stdev, solution
	 */
	void print_generation(int generation, Population * population);

	/*
	 * prints the best solution of all islands.
	 * A line with this format will be printed to the output file.
	 *
	 * bestfitness, totalfitness, meanfitness, fitness_stdev, time, solution
	 *
	 */
	void print_final_results(Population * population);

	void print(Individual * individual);

	/*
	 * prints the header info.
	 */
	void print_header();

	void eval(double time);
	void selection(double time);
	void crossover(double time);
	void replacement(double time);
	void mutation(double time);

	/*
	 * Start a timer.
	 */
	void start();

	/*
	 * Stop the timer.
	 */
	void stop();

	/*
	 * Get the time consumed by the execution.
	 */
	double time();

	/*
	 * Just print the configuration.
	 */
	void print_conf();

	void add_execution();
};

#endif /* OUTPUT_H_ */
