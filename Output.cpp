/*
 * Output.cpp
 *
 *  Created on: 6 Dec 2017
 *      Author: fali
 */

#include "Output.h"

Output::~Output() {
	this->generation_file.close();
	this->solution_file.close();
}

Output::Output(Problem* problem, Config* config) {
	this->problem = problem;
	this->config = config;
	this->t0 = 0;
	this->t1 = 0;

	this->detail = config->getInt("SaveDetail") > 0;

	this->generation_file.open(config->getProperty("OutputGenerationFile"),
			std::ofstream::out | std::ofstream::app);

	this->solution_file.open(config->getProperty("OutputSolutionFile"),
				std::ofstream::out | std::ofstream::app);
}

void Output::print_header() {
	if (this->detail) {
		this->generation_file << "thread_id" << SEPARATOR << "generation" << SEPARATOR
				<< "fitness" << SEPARATOR << "total_fitness" << SEPARATOR
				<< "mean_fitness" << SEPARATOR << "stdev_fitness" << SEPARATOR
				<< "phenotype" << endl;
	}
}
/*
 * print the current status to the file.
 * A line with this format will be printed to the output file.
 *
 * threadid, generation, bestfitness, totalfitness, meanfitness, fitness_stdev, solution
 */
void Output::print_generation( int generation, Population* population) {
	if (this->detail) {
		std::thread::id this_id = std::this_thread::get_id();
		Individual * individual = population->best();

		std::lock_guard<std::mutex> guard(m);

		this->generation_file << this_id << SEPARATOR << generation << SEPARATOR
				<< individual->fitness() << SEPARATOR
				<< population->total_fitness() << SEPARATOR
				<< population->mean_fitness() << SEPARATOR
				<< population->stdev_fitness() << SEPARATOR
				<< problem->decode(individual) << endl;

	}

}

void Output::print_final_results(Population* population) {
	this->solution_file << "---" << endl;
	Individual * individual = population->best();
	this->solution_file << "fitness" << SEPARATOR
				<< "total_fitness" << SEPARATOR
				<< "mean_fitness" << SEPARATOR
				<< "stdev_fitness" << SEPARATOR
				<< "time" << SEPARATOR
				<< "solution" << endl;
	this->solution_file << individual->fitness() << SEPARATOR
			<< population->total_fitness() << SEPARATOR
			<< population->mean_fitness() << SEPARATOR
			<< population->stdev_fitness() << SEPARATOR << time() << SEPARATOR
			<< problem->decode(individual) << endl;
	this->solution_file << "---" << endl;
}

void Output::print(Individual * individual) {
	this->solution_file << "Final winner: [" << individual << "] final solution: ["<<problem->decode(individual)<<"]"<< endl;
}
void Output::start() {
	t0 = clock();
}

void Output::stop() {
	t1 = clock();
}

double Output::time() {
	return (double(t1 - t0) / CLOCKS_PER_SEC);
}
