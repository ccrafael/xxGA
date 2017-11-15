/*
 * Population.cpp
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */
#include "Population.h"
#include "Environment.h"
#include "Util.h"
#include "ga.h"

Population::Population(int size) {
	Population::size = size;
}

Population::~Population() {
	for (int i = 0; i < size; i++) {
		delete Population::individuals.at(i);
	}
}

void Population::init() {
	for (int i = 0; i < size; i ++) {
		Population::individuals.push_back(new Individual(GA::GenesNumber));
	}
}

int Population::menor() {
	double menor = 0;

	for (unsigned int i = 1; i < Population::individuals.size(); i++ ) {
		if (individuals.at(i)->getFitness() < individuals.at(menor)->getFitness()) {
			menor = i;
		}
	}

	return menor;
}
/**
 * obtiene el mayor dado un array de posiciones de individuos
 */
int Population::mayor(vector<int> ind) {
	int mayor = ind[0];
	for (unsigned int i = 1; i < ind.size(); i++ ) {
		if (individuals.at(ind[i])->getFitness() > individuals.at(mayor)->getFitness()) {
			mayor = ind[i];
		}
	}
	return mayor;
}


void Population::generationReplacement(IContainer newgeneration) {

	int i = menor();
	Individual * to_kill = individuals.at(i);
	individuals[i] = newgeneration.at(0);
	delete to_kill;
}

IContainer Population::getIndividuals() {
	return individuals;
}

/**
 * binary tournament
 */
IContainer Population::parentsSelection() {
	switch (GA::ParentSelectionType) {
		case 1: {
			return binaryTournament();
		}

		default: {
			return binaryTournament();
		}
	}
}

IContainer Population::binaryTournament() {
	int i = 0;
	IContainer parents;
	vector<int> vec;
	bool present = false;
	while (i < 2 ) {
		int index = mayor(Util::random(GA::IndividualsNumber, GA::TournamentSize) );

		// no se permiten padres repetidos
		for (int j = 0; j < vec.size(); j++) if (present = (vec[j]==index)) break;

		if (!present) {
			vec.push_back(index);
			parents.push_back(Population::individuals.at(index));
			i++;
		}
	}
	return parents;
}

ostream& operator<< (ostream& os, Population * po) {
	for (unsigned int i = 0; i < po->getIndividuals().size(); i++) {
		os << "  "<< i<<":"<<po->getIndividuals().at(i) << endl;
	}
	return os;
}

ostream& operator<< (ostream& os, Population po) {
	for (unsigned int i = 0; i < po.getIndividuals().size(); i++) {
		os << "  "<< i<<":"<<po.getIndividuals().at(i) << endl;
	}
	return os;
}
