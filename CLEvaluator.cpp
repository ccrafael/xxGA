/*
 * CLEvaluator.cpp
 *
 *  Created on: 17 Dec 2017
 *      Author: fali
 */

#include "CLEvaluator.h"

log4cxx::LoggerPtr CLEvaluator::logger(
		log4cxx::Logger::getLogger("clEvaluator"));

int CLEvaluator::count = 0;
std::mutex CLEvaluator::sync_mutex;
std::condition_variable CLEvaluator::sync;

CLEvaluator::CLEvaluator(Config * config, double * args, int nargs, int num_threads) {

	this->args = args;
	this->num_threads = num_threads;
	this->nargs = nargs;

	//get all platforms (drivers)
	cl::Platform::get(&all_platforms);
	if (all_platforms.size() == 0) {
		throw runtime_error(
				" No OpenCL platforms found. Check OpenCL installation!");
	}

	cl::Platform default_platform = all_platforms[config->getInt("Platform")];

	LOG4CXX_INFO(logger,
			"OpenCL enabled using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>());
	LOG4CXX_INFO(logger, "\n");

	//get default device of the default platform
	default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
	if (all_devices.size() == 0) {
		throw runtime_error(
				"No OpenCL devices found. Check OpenCL installation! ");
	}
	cl::Device default_device = all_devices[config->getInt("Device")];

	LOG4CXX_INFO(logger,
			"Using device: " << default_device.getInfo<CL_DEVICE_NAME>());

	LOG4CXX_INFO(logger, "\n");

	clcontext = new cl::Context( { default_device });

	string kernel_source = kernel_code(config->getProperty("KernelCodeFile"));
	LOG4CXX_INFO(logger, "Adding kernel source: \n"<<kernel_source);

	sources.push_back( { kernel_source.c_str(), kernel_source.length() });

	program = new cl::Program(*clcontext, sources);

	if (program->build( { default_device }) != CL_SUCCESS) {
		std::stringstream out;
		out << " Error building: "
				<< program->getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device)
				<< "\n";

		throw runtime_error(out.str());
	}

	//create queue to which we will push commands for the device.
	queue = new cl::CommandQueue(*clcontext, default_device);

}

void CLEvaluator::evaluate(IContainer * container) {

	std::unique_lock<std::mutex> lk(sync_mutex);

	// copy the references to be evaluated
	std::copy (container->begin(), container->end(), std::back_inserter(to_eval));

	// check if evaluation is possible
	if (count < num_threads - 1) {
		count++;
		sync.wait(lk);
	} else {
		// evaluate all
		evaluate();

		// wake up all threads
		count = 0;
		sync.notify_all();
	}

}

void CLEvaluator::evaluate() {
	int num_individuals = to_eval.size();
	int genes = args[0];
	int numvars = args[4];

	// create buffers on the device
	// TODO review if buffer should be created only once

	// buffer for genotype and N individuals
	cl::Buffer buffer_genotype(*clcontext, CL_MEM_READ_WRITE,
			sizeof(cl_char) * genes * num_individuals);

	// buffer for int args
	cl::Buffer buffer_args(*clcontext, CL_MEM_READ_WRITE,
			sizeof(cl_double) * nargs);

	// buffer to collect the fitness
	cl::Buffer buffer_fitness(*clcontext, CL_MEM_READ_WRITE,
			sizeof(cl_double) * num_individuals);

	// TODO find out a better way to reserve a variable buffer to decode inds
	cl::Buffer buffer_decode(*clcontext, CL_MEM_READ_WRITE,
			sizeof(cl_double) * num_individuals * numvars);

	cl_int err = CL_SUCCESS;

	char genotype[num_individuals * genes];

	// dump all the data into a unique array to pass to the gpu
	for (int i = 0; i < num_individuals; i++) {
		Individual * ind = to_eval.at(i);

		// this  representation is gray if we want to compare
		// opencl will decode to binary, gpu must works
		GenotypeBit * g = ind->get_genotype();

		for (int j = 0; j < genes; j++) {
			genotype[(i * genes) + j] = g->at(j);
		}
	}

	//write the buffers to the device
	queue->enqueueWriteBuffer(buffer_genotype, CL_TRUE, 0,
			sizeof(cl_char) * num_individuals * genes, genotype);
	queue->enqueueWriteBuffer(buffer_args, CL_TRUE, 0,
			sizeof(cl_double) * nargs, args);

	cl::Kernel kernel_add(*program, "evaluate", &err);

	if (err != CL_SUCCESS) {
		throw runtime_error("Error creating kernel.");
	}

	kernel_add.setArg(0, buffer_genotype);
	kernel_add.setArg(1, buffer_args);
	kernel_add.setArg(2, buffer_fitness);

	// decode buffer not needed but we need it for internal calcs
	kernel_add.setArg(3, buffer_decode);

	queue->enqueueNDRangeKernel(kernel_add, cl::NullRange,
			cl::NDRange(num_individuals), cl::NullRange);

	if (queue->flush() != CL_SUCCESS) {
		throw runtime_error("Error executing kernel.");
	}

	double fitness[num_individuals];
	double decode[num_individuals * numvars];

	//read the results
	queue->enqueueReadBuffer(buffer_fitness, CL_TRUE, 0,
			sizeof(cl_double) * num_individuals, fitness);

	//TODO find out a better way to pass a decode buffer
	queue->enqueueReadBuffer(buffer_decode, CL_TRUE, 0,
			sizeof(cl_double) * num_individuals * numvars, decode);

	for (int i = 0; i < num_individuals; i++) {
		LOG4CXX_DEBUG(logger, "Opencl evaluation: "<<i<<": " << fitness[i]);

		Individual * ind = to_eval.at(i);

		ind->fitness(fitness[i]);
		ind->setEvaluated(true);
	}

	to_eval.clear();
}

string CLEvaluator::kernel_code(string file_path) {
	std::ifstream t(file_path);
	std::string str((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());

	return str;
}

CLEvaluator::~CLEvaluator() {
	// TODO Auto-generated destructor stub
}

