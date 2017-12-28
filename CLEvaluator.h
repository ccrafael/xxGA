/*
 * CLEvaluator.h
 *
 *  Created on: 17 Dec 2017
 *      Author: fali
 */

#ifndef CLEVALUATOR_H_
#define CLEVALUATOR_H_

#include <chrono>
#include <queue>
#include <CL/cl.hpp>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <condition_variable>
#include <mutex>

#include "IContainer.h"
#include "log4cxx/logger.h"
#include "Config.h"

struct executor {
	cl::Buffer * buffer_genotype;
	cl::Buffer * buffer_args;
	cl::Buffer * buffer_fitness;
	cl::CommandQueue * queue;
	cl::Kernel * kernel_add;
	char * host_genotype;
	double * host_fitness;
	double * host_decode;
};

class CLEvaluator {
	static log4cxx::LoggerPtr logger;
	cl::Context * clcontext;
	cl::Program * program;
	std::vector<cl::Device> all_devices;
	std::vector<cl::Platform> all_platforms;
	cl::Program::Sources sources;

	cl::Platform default_platform;
	cl::Device default_device;

	double * args;
	int nargs;
	int num_threads;
	int max_individuals;

	// global vars for sincronization
	static int count;
	static std::mutex sync_mutex;
	static std::condition_variable sync;

	std::queue<executor*> executors_queue;

	std::chrono::time_point<std::chrono::steady_clock> t0;
	std::chrono::time_point<std::chrono::steady_clock> t1;

	int tcount = 0;
	double tsum = 0;

	void clevaluate(IContainer * container, executor *e);
	void init_gpu(Config * config);
	void create_kernels(Config * config, int i);
public:
	CLEvaluator(Config * config, double * args, int nargs, int num_threads, int max_individuals);
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
