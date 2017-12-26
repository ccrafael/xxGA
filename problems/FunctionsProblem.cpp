#include <log4cxx/helpers/messagebuffer.h>
#include <cmath>
#include <sstream>

#include "../Config.h"
#include "../exception/ProblemException.h"
#include "../GenotypeBit.h"
#include "../GenotypeNumber.h"
#include "../Individual.h"
#include "../Util.h"
#include "FunctionsProblem.h"
#include <math.h>

using namespace std;

#define PI 3.14159265

log4cxx::LoggerPtr FunctionsProblem::plogger(
		log4cxx::Logger::getLogger("functionsProblem"));

/**
 * Schwefel function
 * The Schwefe's function is another widely used n-dimensional function, usually constrained so
 * taht each variable lies in the range -65.536 <= x <= 65.536
 */
std::function<double(vector<double>)> FunctionsProblem::schwefel =
		[](vector<double> vals) {
			double val = 0;
			for (unsigned int i = 0; i < vals.size(); i++) {
				double sum = 0;
				for (unsigned int j = 0; j <= i; j++ ) {
					sum += vals[j];
				}
				val += pow(sum, 2);
			}

			return val;
		};

/*
 * Rosenbrock function.
 *
 * https://en.wikipedia.org/wiki/Rosenbrock_function
 *
 */
std::function<double(vector<double>)> FunctionsProblem::rosenbrock =
		[](vector<double> vals) {
			double val = 0;
			for (unsigned int i = 0; i < vals.size()-1; i++) {
				double xi = vals[i];
				val += 100 * pow(vals[i-1] - pow(xi, 2), 2) + pow(1-xi, 2);
			}

			return val;
		};

/**
 * The sphere model
 * The sphere model is an ndimesional function in the cotinuous domain. The minimum has value 0.0 at 0.0.
 */
std::function<double(vector<double>)> FunctionsProblem::sphere =
		[](vector<double> vals) {
			double val = 0;
			for (unsigned int i = 0; i < vals.size(); i++) {
				val += pow(vals.at(i), 2);
			}

			return val;
		};

std::function<double(vector<double>)> FunctionsProblem::ackley =
		[](vector<double> vals) {
			int n = vals.size();
			double v1 = 0;
			double v2 = 0;
			for (int i = 0; i < n; i++) {
				v1 += pow(vals[i], 2);
				v2 += cos(2*PI*vals[i]);
			}

			double exp1 = exp( -0.2 * sqrt((1.0/n)*v1) );
			double exp2 = exp((1.0/n)*v2);
			double exp3 = exp(1);

			double result = (-20 * exp1) - exp2 +20 + exp3;

			return result;
		};

std::function<double(vector<double>)> FunctionsProblem::rastrigin =
		[](vector<double> vals) {
			double s = 0;
			int n = vals.size();
			for (int i = 0; i < n; i++) {
				s += pow(vals[i], 2) - (10 * cos(2*PI*vals[i]));
			}
			return n*10 + s;
		};

FunctionsProblem::FunctionsProblem(Config * config) :
		Problem(config) {

	if (configProblem == nullptr) {
		throw ProblemException(
				" Function problem needs a configuration problem file.");
	}

	this->num_vars = configProblem->getInt("num_vars");

	this->configvar = new variable[num_vars];

	int numgenes = config->getInt("NumberGenes");
	int numbits = numgenes / num_vars;

	double xmin = configProblem->getDouble("xmin");
	double xmax = configProblem->getDouble("xmax");

	for (int i = 0; i < num_vars; i++) {

		configvar[i].bits = numbits;
		configvar[i].count = 0;
		configvar[i].min = xmin;
		configvar[i].max = xmax;

		if (configvar[i].min > configvar[i].max) {
			throw ProblemException("Var min greater than max.");
		}
		configvar[i].step = (configvar[i].max - configvar[i].min)
				/ pow(2, configvar[i].bits);
	}

	// OpenCl startup
	if (configProblem->getInt("OpenCL")) {
		int nargs = 5;
		double * args = new double[nargs];
		args[0] = numgenes;
		args[1] = configProblem->getDouble("xmin");
		args[2] = (configProblem->getDouble("xmax") - args[1])/pow(2, numbits); //steps
		args[3] = numbits;
		args[4] = num_vars;

		clEvaluator = new CLEvaluator(configProblem, args, 5, config->getInt("NumberIsles"));
	}

	// 1 schwefel, 2 ackley, 3 sphere, 4 rastrigin
	switch (configProblem->getInt("function")) {
	case 1:
		function = schwefel;
		break;
	case 2:
		function = ackley;
		break;
	case 3:
		function = sphere;
		break;
	case 4:
		function = rastrigin;
		break;
	case 5:
		function = rosenbrock;
		break;
	default:
		throw ProblemException("Unknown function.");
	}

}

FunctionsProblem::~FunctionsProblem() {
	delete configvar;
}

double FunctionsProblem::evaluate(Individual * individual) {
	std::vector<double> d = decode(individual->get_genotype());

	double val = function(d);
	// to minimize we must invert the function
	// the algorithm is always increasing fitnesss
	return 1 / (1 + val);

}

string FunctionsProblem::decode(Individual * individual) {
	stringstream aux;
	int offset = 0;
	vector<bool> v = individual->get_genotype()->grayToBinary();
	for (int i = 0; i < num_vars; i++) {
		aux << "x" << i << ": "
				<< dec(v, offset, configvar[i].bits, configvar[i].min,
						configvar[i].max, configvar[i].step) << " ";
		offset += configvar[i].bits;
	}
	return aux.str();
}

/**
 * get a var value inside a genotype
 */
double FunctionsProblem::dec(vector<bool> gens, int offset, int bits,
		double min, double max, double step) {
	int index = Util::b2i(gens, offset, bits);
	return (step * index) + min;
}

/**
 * decode the whole genotype as array of double
 */
vector<double> FunctionsProblem::decode(GenotypeBit * genotype) {
	vector<double> result;
	vector<bool> binary = genotype->grayToBinary();

	int offset = 0;
	for (int i = 0; i < num_vars; i++) {
		result.push_back(
				dec(binary, offset, configvar[i].bits,
						configvar[i].min, configvar[i].max, configvar[i].step));
		offset += configvar[i].bits;
	}
	return result;
}

void FunctionsProblem::clevaluate(IContainer * individuals) {
	this->clEvaluator->evaluate(individuals);
}
