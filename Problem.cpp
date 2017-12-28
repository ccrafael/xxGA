/*
 * Problem.cpp
 *
 *  Created on: 8 Dec 2017
 *      Author: fali
 */

#include "Problem.h"

log4cxx::LoggerPtr Problem::logger(log4cxx::Logger::getLogger("problem"));

Problem::Problem(Config* config) :
		config(config) {
	try {
		string filename(config->find(PROBLEM_FILE_CONFIG));
		this->configProblem = new Config(filename);
	} catch (NotFoundException &e) {
		LOG4CXX_WARN(logger, "NO problem configuration file.");
		this->configProblem = nullptr;
	}
}

Problem::~Problem() {
	if (this->configProblem != nullptr) {
		delete this->configProblem;
	}
}

void Problem::clevaluate(IContainer * individuals) {
		LOG4CXX_ERROR(logger, "Calling OpenCL implementation. But OpenCL was not implemented.");
		throw std::runtime_error("OpenCL evaluation not implemented.");
}
