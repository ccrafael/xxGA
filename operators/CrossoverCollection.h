/*
 * CrossoverCollection.h
 *
 * This header file contains a collection of crossover operators. Each one is a
 * lambda expression implemented into the respective cpp file.
 *
 * Each operator takes a
 *
 *  Created on: 8 Dec 2017
 *      Author: fali
 */

#ifndef OPERATORS_CROSSOVERCOLLECTION_H_
#define OPERATORS_CROSSOVERCOLLECTION_H_

#include <functional>
#include "../IContainer.h"
#include "../Individual.h"
#include "../GenotypeBit.h"
#include "../exception/OperatorException.h"

namespace crossover {
/**
 * Do nothing.
 */
const std::function<IContainer* (IContainer*)> empty = [](IContainer* i) {
	return new IContainer();
};

/*
 * Crossover of two individuals. From two individuals
 * id1: aaaaaaa
 * id2: bbbbbbb
 * it returns two new indiviuals like
 * nid1: aaaabbb
 * nid2: bbbaaaa
 * the cross point is taken randomly.
 */
const std::function<IContainer* (IContainer*)> basic =
		[](IContainer* i) {

			if (i == nullptr || i->size() < 2) {
				throw OperatorException("Basic crossover operator expect 2 indivividuals.");
			}

			GenotypeBit * p1 = i->at(0)->get_genotype();
			GenotypeBit * p2 = i->at(1)->get_genotype();

			int size = p1->size();


			int cross_point = Util::rand(size-2) +1;

			GenotypeBit * newGenotype1 = new GenotypeBit();
			GenotypeBit * newGenotype2 = new GenotypeBit();

			for (int i = 0; i < size; i++) {


				newGenotype1->push_back( i<cross_point?p1->at(i):p2->at(2));
				newGenotype2->push_back( i>cross_point?p1->at(i):p2->at(2));

			}


			IContainer * container = new IContainer();

			container->push_back(new Individual(newGenotype1));
			container->push_back(new Individual(newGenotype2));

			return container;
		};
}

#endif /* OPERATORS_CROSSOVERCOLLECTION_H_ */
