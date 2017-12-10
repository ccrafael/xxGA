#include "BagProblem.h"

#include <log4cxx/helpers/messagebuffer.h>
#include <cmath>
#include <sstream>
#include <algorithm>

#include "../Config.h"
#include "../exception/ProblemException.h"
#include "../GenotypeBit.h"
#include "../GenotypeNumber.h"
#include "../Individual.h"
#include "../Util.h"

using namespace std;

log4cxx::LoggerPtr BagProblem::plogger(
		log4cxx::Logger::getLogger("BagProblem"));

BagProblem::BagProblem(Config * config) :
		Problem(config) {

	if (configProblem == nullptr) {
		throw ProblemException(
				" BagProblem needs a configuration problem file.");
	}

	this->max_size = configProblem->getDouble("size");
	string allbins(configProblem->getProperty("bins"));

	char *cstr = new char[allbins.length() + 1];
	strcpy(cstr, allbins.c_str());

	char * pch;
	pch = strtok(cstr, ",");
	string::size_type sz;

	while (pch != NULL) {
		bins.push_back(stod(string(pch), &sz));
		pch = strtok(NULL, ",");

	}

	for_each(bins.begin(), bins.end(), [this](double b) { LOG4CXX_INFO(logger,  " ["<< b<< "]");});
}

double BagProblem::evaluate(Individual * individual) {
	GenotypeBit* g = individual->get_genotype();

	double sum = 0;
	double count = 0;
	for (int i = 0; i < g->size(); i++) {
		sum += g->at(i) ? bins.at(i) : 0;
		count += g->at(i)?1:0;
	}
	double penalty = 1;
	if (sum > max_size) {
		penalty = pow(10, max_size - sum);
	}

	double fitness =  (1/(1+pow(max_size - sum, 2))) * count * penalty ;

	LOG4CXX_DEBUG(logger, " evaluating: "<< individual<<" sum: "<<sum<<" fitness: "<<fitness);
	return fitness;
}

string BagProblem::decode(Individual * individual) {
	stringstream aux;

	GenotypeBit* g = individual->get_genotype();
	aux<<"[";
	for (int i = 0; i < g->size(); i++) {
		if (g->at(i)) {
			aux<< bins.at(i)<<" ";
		}
	}
	aux<<"]";
	return aux.str();
}

BagProblem::~BagProblem() {
}

