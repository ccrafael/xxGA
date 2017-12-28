/*
 * Output.cpp
 *
 *  Created on: 6 Dec 2017
 *      Author: fali
 */

#include "Output.h"

log4cxx::LoggerPtr Output::logger(log4cxx::Logger::getLogger("output"));

#include <iostream>
#include <ctime>

Output::~Output() {
	this->generation_file.close();
	this->solution_file.close();
}

Output::Output(Problem* problem, Config* config) {
	this->problem = problem;
	this->config = config;

	this->eval_count = 0;
	this->eval_sum = 0;

	this->selection_count = 0;
	this->selection_sum = 0;

	this->crossover_count = 0;
	this->crossover_sum = 0;

	this->mutation_count = 0;
	this->mutation_sum = 0;

	this->replacement_count = 0;
	this->replacement_sum = 0;

	this->numgen_to_print = config->getInt("OuputEachGenerations");

	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	std::time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y-%I-%M-%S", timeinfo);
	std::string str(buffer);

	this->generation_file.open(
			config->getProperty("OutputGenerationFile") + "-" + str + ".csv",

			std::ofstream::out | std::ofstream::trunc);

	this->solution_file.open(
			config->getProperty("OutputSolutionFile") + "-" + str + ".csv",

			std::ofstream::out | std::ofstream::trunc);
}

void Output::print_header() {
	this->generation_file << "thread_id" << SEPARATOR << "generation"
			<< SEPARATOR << "fitness" << SEPARATOR << "mean_fitness"<< endl;
}

/*
 * print the current status to the file.
 * A line with this format will be printed to the output file.
 *
 * threadid, generation, bestfitness, totalfitness, meanfitness, fitness_stdev, solution
 */
void Output::print_generation(int generation, Population* population) {
	if (generation % numgen_to_print == 0) {
		LOG4CXX_TRACE(logger, "Get thread id.");
		std::thread::id this_id = std::this_thread::get_id();

		LOG4CXX_TRACE(logger, "Get the best.");
		Individual * individual = population->best();

		LOG4CXX_TRACE(logger, "Get statistics.");

		double mean = population->mean_fitness();
		//double stddev = population->stdev_fitness(mean);

		LOG4CXX_TRACE(logger, "Print.");

		std::lock_guard<std::mutex> guard(m);
		this->generation_file << this_id << SEPARATOR << generation << SEPARATOR
				<< individual->fitness() << SEPARATOR << mean << endl;
	}
}

void Output::print_conf() {
	this->solution_file << "Algorithm main configuration " << endl;
	this->solution_file << "-------------------------------------- " << endl;
	this->solution_file << "NumberIsles:         "
			<< config->getProperty("NumberIsles") << endl;
	this->solution_file << "NumberIndividuals:   "
			<< config->getProperty("NumberIndividuals") << endl;
	this->solution_file << "NumberGenerations:   "
			<< config->getProperty("NumberGenerations") << endl;
	this->solution_file << "NumberGenes:         "
			<< config->getProperty("NumberGenes") << endl;
	this->solution_file << "MigrationEveryGenerations: "
			<< config->getProperty("MigrationEveryGenerations") << endl;
	this->solution_file << "NumberMigrants:      "
			<< config->getProperty("NumberMigrants") << endl;
	this->solution_file << "MigrationType:       "
			<< config->getProperty("MigrationType") << endl;
	this->solution_file << "ParentSelectionType: "
			<< config->getProperty("ParentSelectionType") << endl;
	this->solution_file << "NumParents:          "
			<< config->getProperty("NumParents") << endl;
	this->solution_file << "TournamentSize:      "
			<< config->getProperty("TournamentSize") << endl;
	this->solution_file << "CrossoverType:       "
			<< config->getProperty("CrossoverType") << endl;
	this->solution_file << "CrossoverType:       "
			<< config->getProperty("CrossoverType") << endl;
	this->solution_file << "MutationType:        "
				<< config->getProperty("MutationType") << endl;
	this->solution_file << "MutationRate:        "
			<< config->getProperty("MutationRate") << endl;
	this->solution_file << "MutationProbability: "
				<< config->getProperty("MutationProbability") << endl;
	this->solution_file << "SurvivorSelectionType: "
			<< config->getProperty("SurvivorSelectionType") << endl;
	this->solution_file << "EvaluationType:       "
				<< config->getProperty("EvaluationType") << endl;
}

void Output::print_final_results(Population* population) {

	this->solution_file << "---" << endl;
	Individual * individual = population->best();
	this->solution_file << "fitness" << SEPARATOR << "total_fitness"
			<< SEPARATOR << "mean_fitness" << SEPARATOR << "stdev_fitness"
			<< SEPARATOR << "time" << endl;

	this->solution_file << individual->fitness() << SEPARATOR
			<< population->total_fitness() << SEPARATOR
			<< population->mean_fitness() << SEPARATOR << time() << SEPARATOR
			<< problem->decode(individual) << endl;
	this->solution_file << "---" << endl;
}

void Output::print(Individual * individual) {
	this->solution_file << "Final winner: [" << individual
			<< "] final solution: [" << problem->decode(individual) << "]"
			<< endl;
	this->solution_file << "Time:         [" << time()/1000 << "] s" << endl;

	this->solution_file << "Sele time avg:[" << (selection_sum/selection_count)/1000 << "] s" << endl;
	this->solution_file << "Cros time avg:[" << (crossover_sum/crossover_count)/1000 << "] s" << endl;
	this->solution_file << "Muta time avg:[" << (mutation_sum/mutation_count)/1000 << "] s" << endl;
	this->solution_file << "Eval time avg:[" << (eval_sum/eval_count)/1000 << "] s" << endl;
	this->solution_file << "Repl time avg:[" << (replacement_sum/replacement_count)/1000 << "] s" << endl;
}

void Output::start() {
	t0 = chrono::steady_clock::now();
}

void Output::stop() {
	t1 = chrono::steady_clock::now();
}

//
void Output::eval(double time) {
	std::lock_guard<std::mutex> guard(m);
	eval_count ++;
	eval_sum += time;
}

void Output::selection(double time) {
	std::lock_guard<std::mutex> guard(m);
	selection_count ++;
	selection_sum += time;
}

void Output::mutation(double time) {
	std::lock_guard<std::mutex> guard(m);
	mutation_count ++;
	mutation_sum += time;
}

void Output::replacement(double time) {
	std::lock_guard<std::mutex> guard(m);
	replacement_count ++;
	replacement_sum += time;
}

void Output::crossover(double time) {
	std::lock_guard<std::mutex> guard(m);
	crossover_count ++;
	crossover_sum += time;
}

/*
 * time in ms
 */
double Output::time() {
	return chrono::duration<double, milli>(t1 - t0).count();
}
