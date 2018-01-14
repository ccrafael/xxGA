#include <log4cxx/helpers/messagebuffer.h>
#include <cmath>
#include <sstream>

#include <sstream>

#include "Image.h"

using namespace std;

log4cxx::LoggerPtr ImageProblem::plogger(
		log4cxx::Logger::getLogger("imageProblem"));

ImageProblem::ImageProblem(Config * config) :
		Problem(config) {

	if (configProblem == nullptr) {
		throw ProblemException(
				" Function problem needs a configuration problem file.");
	}

	std::string image_path(configProblem->getProperty("image_path"));
	numgenes = config->getInt("NumberGenes");

	ifstream file(image_path);

	rows = configProblem->getInt("rows");
	cols = configProblem->getInt("cols");

	LOG4CXX_DEBUG(plogger,
			"Loading image: "<<image_path<<" with rows: "<<rows<<" and cols: "<<cols);

	image = new short*[rows];
	for (int i = 0; i < rows; i++) {
		image[i] = new short[cols];
	}

	LOG4CXX_DEBUG(plogger, "loading ...");
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			file >> image[i][j];
		}
	}

	LOG4CXX_DEBUG(plogger, "Image loaded");
	if (plogger->isDebugEnabled()) {
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				cout << image[i][j] << " ";
			}
			cout << endl;
		}
	}
}

double ImageProblem::evaluate(Individual * individual) {
	double value = 0;
	GenotypeNumber<short> * g =
			(GenotypeNumber<short>*) individual->get_genotype();

	int conv_size = 1;
	for (int i = conv_size; i < g->size() - conv_size; i++) {
		double dif = 0;
		for (int j = 0; j < cols; j++) {
			double err = 0;
			for (int h = i - conv_size; h <= i + conv_size; h++) {
				err += pow(image[g->at(h)][j] - image[g->at(i)][j], 2);
			}
			dif += err;
		}

		value += dif;
	}

	return 1 / (1 + value);
}


string ImageProblem::decode(Individual * individual) {
	stringstream aux;
	GenotypeNumber<short> * g =
			(GenotypeNumber<short>*) individual->get_genotype();
	for (int i = 0; i < g->size(); i++) {
		aux << g->at(i) << ",";
	}
	return aux.str();
}

ImageProblem::~ImageProblem() {
	delete image;

}

Individual * ImageProblem::create_new_individual(int birth) {
	vector<int> permutation = Util::random(numgenes, numgenes);
	GenotypeNumber<short> * genotype = new GenotypeNumber<short>();
	for (int i = 0; i < numgenes; i++) {
		genotype->push_back(permutation[i]);
	}

	LOG4CXX_TRACE(plogger, "Create individual: "<<genotype);
	return new Individual(genotype, birth);

}
