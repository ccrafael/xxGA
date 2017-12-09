/*
 * Output.cpp
 *
 *  Created on: 6 Dec 2017
 *      Author: fali
 */

#include "Output.h"

Output::~Output() {
	this->file.close();
}

Output::Output(Problem* problem, Config* config) {
	this->problem = problem;
	this->config = config;
	this->t0 = 0;
	this->t1 = 0;

	this->detail = config->getInt("SaveDetail") > 0;
	this->file.open(config->getProperty("OutputFile"),
			std::ofstream::out | std::ofstream::app);
}

void Output::print_header() {
	if (this->detail) {
		this->file << "thread_id" << SEPARATOR << "generation" << SEPARATOR
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
void Output::print_generation(int generation, Population* population) {
	if (this->detail) {
		std::thread::id this_id = std::this_thread::get_id();
		Individual * individual = population->best();
		mutex.lock();
		this->file << this_id << SEPARATOR << generation << SEPARATOR
				<< individual->fitness() << SEPARATOR
				<< population->total_fitness() << SEPARATOR
				<< population->mean_fitness() << SEPARATOR
				<< population->stdev_fitness() << SEPARATOR
				<< problem->decode(individual) << endl;
		mutex.unlock();
	}

}

void Output::print_final_results(Population* population) {
	this->file << "---" << endl;
	Individual * individual = population->best();
	this->file << "fitness" << SEPARATOR
				<< "total_fitness" << SEPARATOR
				<< "mean_fitness" << SEPARATOR
				<< "stdev_fitness" << SEPARATOR
				<< "time" << SEPARATOR
				<< "solution" << endl;
	this->file << individual->fitness() << SEPARATOR
			<< population->total_fitness() << SEPARATOR
			<< population->mean_fitness() << SEPARATOR
			<< population->stdev_fitness() << SEPARATOR << time() << SEPARATOR
			<< problem->decode(individual) << endl;
	this->file << "---" << endl;
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
