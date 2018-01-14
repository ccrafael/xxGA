#include <log4cxx/helpers/messagebuffer.h>
#include <cmath>
#include <sstream>

#include "../Config.h"
#include "../exception/ProblemException.h"
#include "../GenotypeBit.h"
#include "../GenotypeNumber.h"
#include "../Individual.h"
#include "../GenotypeBit.h"
#include "../Util.h"
#include "ParseFunctionProblem.h"

using namespace std;

log4cxx::LoggerPtr ParseFunctionProblem::plogger(
		log4cxx::Logger::getLogger("parseFunctionProblem"));

ParseFunctionProblem::ParseFunctionProblem(Config * config) :
		Problem(config) {

	if (configProblem == nullptr) {
		throw ProblemException(
				" Function problem needs a configuration problem file.");
	}

	this->num_vars = configProblem->getInt("num_vars");

	std::string function(configProblem->getProperty("function"));

	std::string s;
	std::stringstream out;

	// init the string with the vars needed for the function parser
	if (num_vars > 0) {
		out << "x0";
		for (int i = 1; i < num_vars; i++) {
			out << ",x" << i;
		}
		s = out.str();
	} else {
		throw ProblemException("The property num_vars must be ");
	}

	this->configvar = new pvariable[num_vars];

	this->numgenes = config->getInt("NumberGenes");
	int numbits = numgenes / num_vars;

	std::ostringstream aux;

	for (int i = 0; i < num_vars; i++) {

		configvar[i].bits = numbits;
		configvar[i].count = 0;

		aux.clear();
		aux.str("");
		aux << "x" << i << ".min";
		configvar[i].min = configProblem->getDouble(aux.str());

		aux.clear();
		aux.str("");
		aux << "x" << i << ".max";
		configvar[i].max = configProblem->getDouble(aux.str());

		if (configvar[i].min > configvar[i].max) {
			throw ProblemException("Var min greater than max.");
		}
		configvar[i].step = (configvar[i].max - configvar[i].min)
				/ pow(2, configvar[i].bits);
	}

	fparser.AddConstant("pi", 3.1415926535897932);

	int res = fparser.Parse(function, s);
	if (res >= 0) {
		LOG4CXX_ERROR(plogger,
				"Error reading the function: "<< function<<" "<<std::string(res + 7, ' ') << "\n" << fparser.ErrorMsg());

		throw ProblemException("Error reading the function. ");
	}
}

double ParseFunctionProblem::evaluate(Individual * individual) {
	std::vector<double> d = decode((GenotypeBit*)individual->get_genotype());
	double values[d.size()]={0};

	double val = 0;
	for (unsigned int i = 0; i < d.size(); i++) {
		values[i] = d.at(i);

		double sum = 0;
		for (int j = 0; j < i; j++) {
			sum += values[j];
		}
		val += pow(sum, 2);
	}


	double eval = fparser.Eval(values);

	double fitness = 1/(1 + eval);

	LOG4CXX_TRACE(plogger, " i: "<<individual<< " fitness: "<< fitness<< " "<< decode(individual));


	// to minimize we must invert the function
	// the algorithm is always increasing fitnesss
	return fitness;

}

string ParseFunctionProblem::decode(Individual * individual) {
	stringstream aux;
	int offset = 0;
	vector<bool> v = ((GenotypeBit*)individual->get_genotype())->grayToBinary();
	for (int i = 0; i < num_vars; i++) {
		aux << "x" << i << ": "
				<< dec(v, offset,
						configvar[i].bits, configvar[i].min, configvar[i].max,
						configvar[i].step) << " ";
		offset += configvar[i].bits;
	}
	return aux.str();
}

ParseFunctionProblem::~ParseFunctionProblem() {
	delete configvar;
}

/**
 * get a var value inside a genotype
 */
double ParseFunctionProblem::dec(vector<bool> gens, int offset, int bits, double min,
		double max, double step) {
	int index = Util::b2i(gens, offset, bits);
	return (step * index) + min;
}

/**
 * decode the whole genotype as array of double
 */
vector<double> ParseFunctionProblem::decode(GenotypeBit * genotype) {
	stringstream out;
	vector<double> result;
	int offset = 0;
	for (int i = 0; i < num_vars; i++) {
		result.push_back(
				dec(genotype->grayToBinary(), offset, configvar[i].bits,
						configvar[i].min, configvar[i].max, configvar[i].step));
		offset += configvar[i].bits;
	}
	return result;
}



Individual * ParseFunctionProblem::create_new_individual(int birth) {
	return new Individual(new GenotypeBit(numgenes), birth);
}

