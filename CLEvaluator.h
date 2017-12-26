/*
 * CLEvaluator.h
 *
 *  Created on: 17 Dec 2017
 *      Author: fali
 */

#ifndef CLEVALUATOR_H_
#define CLEVALUATOR_H_

#include <CL/cl.hpp>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <condition_variable>
#include <mutex>

#include "IContainer.h"
#include "log4cxx/logger.h"
#include "Config.h"

class CLEvaluator {
	static log4cxx::LoggerPtr logger;
	cl::Context * clcontext;
	cl::Program * program;
	std::vector<cl::Device> all_devices;
	std::vector<cl::Platform> all_platforms;
	cl::Program::Sources sources;
	cl::CommandQueue * queue;

	double * args;
	int nargs;
	int num_threads;

	IContainer to_eval;

	// global vars for sincronization
	static int count;
	static std::mutex sync_mutex;
	static std::condition_variable sync;

	void evaluate();
public:
	CLEvaluator(Config * config, double * args, int nargs, int num_threads);
	virtual ~CLEvaluator();

	/*!
	 * \brief Get the kernel source in case of opencl implementation.
	 *
	 * @return the kernel source.
	 */
	string kernel_code(string file_path);

	/*!
	 * \brief Evaluate a set of individuals using Opencl
	 *
	 */
	void evaluate(IContainer * container);
};

#endif /* CLEVALUATOR_H_ */
