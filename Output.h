/*
 * Output.h
 *
 *  Created on: 6 Dec 2017
 *      Author: fali
 */

#ifndef OUTPUT_H_
#define OUTPUT_H_

#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <ctime>
#include "Config.h"
#include "Population.h"

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
	Config * config;
	Problem * problem;
	ofstream file;
	bool detail;
	std::mutex mutex;
	static constexpr const char* SEPARATOR = ",";
	unsigned t0, t1;

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
	void print_final_results(Population * island);

	/*
	 * prints the header info.
	 */
	void print_header();

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
};

#endif /* OUTPUT_H_ */
