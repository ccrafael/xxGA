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

CLEvaluator::CLEvaluator(Config * config, double * args, int nargs,
		int num_threads, int max_individuals) {

	this->args = args;
	this->num_threads = num_threads;
	this->nargs = nargs;
	this->max_individuals = max_individuals;

	// init the gpu
	init_gpu(config);

	// create a executor for each thread
	for (int i = 0; i < num_threads; i++) {
		// create the kernels
		create_kernels(config, i);
	}
}

void CLEvaluator::create_kernels(Config * config, int i) {
	cl_int err = CL_SUCCESS;
	int genes = args[0];
	int numvars = args[4];
	int num_individuals = max_individuals / num_threads;

	executor * e = new executor;

	// create host memory
	e->host_genotype = new char[num_individuals * genes];
	e->host_fitness = new double[num_individuals];

	e->buffer_genotype = new cl::Buffer(*clcontext,
	CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
			sizeof(cl_char) * genes * num_individuals, e->host_genotype);

	// buffer for int args
	e->buffer_args = new cl::Buffer(*clcontext,
	CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(cl_double) * nargs, args);

	// buffer to collect the fitness
	e->buffer_fitness = new cl::Buffer(*clcontext,
	CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
			sizeof(cl_double) * num_individuals, e->host_fitness);

	//create queue to which we will push commands for the device.
	e->queue = new cl::CommandQueue(*clcontext, default_device);

	e->kernel_add = new cl::Kernel(*program, "evaluate", &err);

	// buffer for genotype and N individuals

	e->kernel_add->setArg(0, *e->buffer_genotype);
	e->kernel_add->setArg(1, *e->buffer_args);
	e->kernel_add->setArg(2, *e->buffer_fitness);

	executors_queue.push(e);

	LOG4CXX_INFO(logger, "OpenCL initialized.");
}

void CLEvaluator::init_gpu(Config * config) {
	//get all platforms (drivers)
	cl::Platform::get(&all_platforms);
	if (all_platforms.size() == 0) {
		throw runtime_error(
				" No OpenCL platforms found. Check OpenCL installation!");
	}

	default_platform = all_platforms[config->getInt("Platform")];

	LOG4CXX_INFO(logger,
			"OpenCL enabled using platform: " << default_platform.getInfo<CL_PLATFORM_NAME>());

	LOG4CXX_INFO(logger, "\n");

	//get default device of the default platform
	default_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
	if (all_devices.size() == 0) {
		throw runtime_error(
				"No OpenCL devices found. Check OpenCL installation! ");
	}
	default_device = all_devices[config->getInt("Device")];

	LOG4CXX_INFO(logger,
			"Using device: " << default_device.getInfo<CL_DEVICE_NAME>());

	LOG4CXX_INFO(logger, "\n");

	clcontext = new cl::Context( { default_device });

	string kernel_source = kernel_code(config->getProperty("KernelCodeFile"));
	LOG4CXX_INFO(logger, "Adding kernel source: \n" << kernel_source);

	sources.push_back( { kernel_source.c_str(), kernel_source.length() });

	program = new cl::Program(*clcontext, sources);

	LOG4CXX_TRACE(logger, "Build the kernel online.");

	if (program->build( { default_device }) != CL_SUCCESS) {
		std::stringstream out;

		LOG4CXX_TRACE(logger,
				"Something weird happened." << program->getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device));
		out << " Error building: "
				<< program->getBuildInfo<CL_PROGRAM_BUILD_LOG>(default_device)
				<< "\n";

		throw runtime_error(out.str());
	}
}

void CLEvaluator::evaluate(IContainer * container) {

	std::unique_lock<std::mutex> lk(sync_mutex);
	executor * e = this->executors_queue.front();
	this->executors_queue.pop();
	lk.unlock();

	clevaluate (container , e);

	lk.lock();
	this->executors_queue.push(e);
	lk.unlock();

}

void CLEvaluator::clevaluate(IContainer * container, executor * e) {
	int num_individuals = container->size();
	int genes = args[0];

	// create buffers on the device
	// TODO review if buffer should be created only once
	LOG4CXX_TRACE(logger, "Start preparing buffers for "<< num_individuals);

	// dump all the data into a unique array to pass to the gpu
	for (int i = 0; i < num_individuals; i++) {
		Individual * ind = container->at(i);

		// this  representation is gray if we want to compare
		// opencl will decode to binary, gpu must works
		GenotypeBit * g = ind->get_genotype();

		for (int j = 0; j < genes; j++) {
			e->host_genotype[(i * genes) + j] = g->at(j);
		}

	}

	LOG4CXX_TRACE(logger, "Data ready. ");

	//write the buffers to the device
	e->queue->enqueueWriteBuffer(*e->buffer_genotype, CL_TRUE, 0,
			sizeof(cl_char) * num_individuals * genes, e->host_genotype);

	LOG4CXX_TRACE(logger, "Write genotype data to device. ");

	e->queue->enqueueNDRangeKernel(*e->kernel_add, cl::NullRange,
			cl::NDRange(num_individuals), cl::NullRange);

	LOG4CXX_TRACE(logger, "Enqueue ND range kernel.  ");

	if (e->queue->finish() != CL_SUCCESS) {
		throw runtime_error("Error executing kernel.");
	}

	//read the results
	e->queue->enqueueMapBuffer(*e->buffer_fitness, CL_TRUE, CL_MAP_WRITE, 0,
			sizeof(cl_double) * num_individuals, 0);

	LOG4CXX_TRACE(logger, "Read fitness buffer.");

	for (int i = 0; i < num_individuals; i++) {
		LOG4CXX_DEBUG(logger,
				"Opencl evaluation: "<<i<<": " << e->host_fitness[i]);

		Individual * ind = container->at(i);

		ind->fitness(e->host_fitness[i]);
		ind->setEvaluated(true);
	}
	LOG4CXX_TRACE(logger, "Copied fitness values. ");
}

string CLEvaluator::kernel_code(string file_path) {
	std::ifstream t(file_path);
	std::string str((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());

	return str;
}

CLEvaluator::~CLEvaluator() {
	while(!this->executors_queue.empty()) {
		executor * e = this->executors_queue.front();
		this->executors_queue.pop();
		delete e->buffer_args;
		delete e->buffer_fitness;
		delete e->buffer_genotype;
		delete e->host_fitness;
		delete e->host_genotype;
		delete e->kernel_add;
		delete e->queue;

		delete e;
	}
	delete clcontext;
	delete program;
}

