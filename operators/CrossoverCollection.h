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
#include <stdexcept>
#include <string>
#include "../IContainer.h"
#include "../Individual.h"
#include "../GenotypeBit.h"
#include "../GenotypeNumber.h"
#include "../exception/OperatorException.h"
#include "../Environment.h"
#include "../Util.h"

namespace crossover {
/**
 * Do nothing.
 */
const std::function<IContainer* (IContainer*, int)> empty =
		[](IContainer* i, int g) {
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
const std::function<IContainer* (IContainer*, int)> onepoint =
		[](IContainer* i, int generation) {

			if (i == nullptr || i->size() < 2) {
				throw OperatorException("Basic crossover operator expect 2 indivividuals.");
			}

			IContainer * container = new IContainer();
			Environment *context = Environment::instance();

			while ((int)container->size() < context->num_offspring) {
				vector<int> parents = Util::random(i->size(), 2);

				GenotypeBit * p1 = dynamic_cast<GenotypeBit*>(i->at(parents[0])->get_genotype());
				GenotypeBit * p2 = dynamic_cast<GenotypeBit*>(i->at(parents[1])->get_genotype());

				if (p1 == nullptr || p2 == nullptr) {
					throw OperatorException("Bad casting");
				}

				int size = p1->size();

				int cross_point = Util::rand(size);

				GenotypeBit *newGenotype1 = new GenotypeBit();
				GenotypeBit *newGenotype2 = new GenotypeBit();

				for (int i = 0; i < size; i++) {
					newGenotype1->push_back( i<cross_point?p1->at(i):p2->at(i));
					newGenotype2->push_back( i>=cross_point?p1->at(i):p2->at(i));
				}

				// be careful individual create its own copy
				container->push_back(new Individual(newGenotype1, generation));
				container->push_back(new Individual(newGenotype2, generation));
			}

			return container;
		};

/*
 * Uniform crossover with a exchange probability.
 */
const std::function<IContainer* (IContainer*, int)> uniformcrossover =
		[](IContainer* i, int generation) {

			if (i == nullptr || i->size() < 2) {
				throw OperatorException("Uniform crossover operator expect 2 individuals.");
			}

			Environment *context = Environment::instance();
			IContainer * container = new IContainer();

			while ((int)container->size() < context->num_offspring) {
				vector<int> parents = Util::random(i->size(), 2);
				GenotypeBit * p1 = dynamic_cast<GenotypeBit*>(i->at(parents[0])->get_genotype());
				GenotypeBit * p2 = dynamic_cast<GenotypeBit*>(i->at(parents[1])->get_genotype());

				if (p1 == nullptr || p2 == nullptr) {
					throw OperatorException("Bad casting");
				}

				int size = p1->size();

				GenotypeBit *newGenotype1 = new GenotypeBit();
				GenotypeBit *newGenotype2 = new GenotypeBit();

				for (int i = 0; i < size; i++) {
					double p = Util::rand();

					newGenotype1->push_back( p < context->exchange_probability?p1->at(i):p2->at(i));
					newGenotype2->push_back( p > context->exchange_probability?p1->at(i):p2->at(i));

				}

				container->push_back(new Individual(newGenotype1, generation));
				container->push_back(new Individual(newGenotype2, generation));
			}
			return container;
		};

const std::function<IContainer* (IContainer*, int)> npoint =
		[](IContainer* i, int g) {
			throw std::runtime_error("npoint crossover not implemented yet ");
			return nullptr;
		};

const std::function<IContainer* (IContainer*, int)> orderedcrossover =
		[](IContainer* i, int g) {

			if (i == nullptr || i->size() < 2) {
				throw OperatorException("Basic crossover operator expect 2 indivividuals.");
			}

			IContainer * container = new IContainer();
			double p = Environment::instance()->exchange_probability;
			int num_offspring = Environment::instance()->num_offspring;

			while ((int)container->size() < num_offspring) {
				vector<int> parents = Util::random(i->size(), 2);

				GenotypeNumber<short> * p1 = dynamic_cast<GenotypeNumber<short>*>(i->at(parents[0])->get_genotype());
				GenotypeNumber<short> * p2 = dynamic_cast<GenotypeNumber<short>*>(i->at(parents[1])->get_genotype());

				if (p1 == nullptr || p2 == nullptr) {
					throw OperatorException("Bad casting");
				}

				int size = p1->size();

				int cross_point = Util::rand(size);

				GenotypeNumber<short> *newGenotype1 = new GenotypeNumber<short>();
				GenotypeNumber<short> *newGenotype2 = new GenotypeNumber<short>();

				if (Util::should_be_changed(p)) {
					set<short> p1added;
					set<short> p2added;

					for (int i = 0; i < cross_point; i++) {

						p1added.insert(p1->at(i));
						p2added.insert(p2->at(i));

						newGenotype1->push_back( p1->at(i));
						newGenotype2->push_back( p2->at(i));
					}

					for (int i = cross_point; i < size; i++) {
						short find_1 =-1;
						short find_2 =-1;
						for (int j = 0; j < size; j++) {
							if (p1added.find(p2->at(j)) == p1added.end()) {
								find_1 = p2->at(j);
								p1added.insert(find_1);
								break;
							}
						}
						for (int j = 0; j < size; j++) {
							if (p2added.find(p1->at(j)) == p2added.end()) {
								find_2 = p1->at(j);
								p2added.insert(find_2);
								break;
							}
						}
						newGenotype1->push_back( find_1);
						newGenotype2->push_back( find_2);

					}
				} else {
					newGenotype1->set(p1);
					newGenotype2->set(p2);
				}

				// be careful individual create its own copy
				container->push_back(new Individual(newGenotype1, g));
				container->push_back(new Individual(newGenotype2, g));
			}

			return container;
		};

const std::function<IContainer* (IContainer*, int)> realordercrossover =
		[](IContainer* ind, int g) {

			if (ind == nullptr || ind->size() < 2) {
				throw OperatorException("Basic crossover operator expect 2 indivividuals.");
			}

			IContainer * container = new IContainer();
			double p = Environment::instance()->exchange_probability;
			int num_offspring = Environment::instance()->num_offspring;

			while ((int)container->size() < num_offspring) {
				vector<int> parents = Util::random(ind->size(), 2);

				GenotypeNumber<short> * p1 = dynamic_cast<GenotypeNumber<short>*>(ind->at(parents[0])->get_genotype());
				GenotypeNumber<short> * p2 = dynamic_cast<GenotypeNumber<short>*>(ind->at(parents[1])->get_genotype());

				if (p1 == nullptr || p2 == nullptr) {
					throw OperatorException("Bad casting");
				}

				int size = p1->size();

				GenotypeNumber<short> *newGenotype1 = new GenotypeNumber<short>();
				GenotypeNumber<short> *newGenotype2 = new GenotypeNumber<short>();

				newGenotype1->set(p1);
				newGenotype2->set(p2);

				if (Util::should_be_changed(p)) {
					vector<int> r = Util::random(size, 2);
					int cp1 = min(r[0], r[1]);
					int cp2 = max(r[0], r[1]);

					set<short> p1added;
					set<short> p2added;

					for (int i = cp1; i <= cp2; i++) {
						p1added.insert(p1->at(i));
						p2added.insert(p2->at(i));
					}

					int i = (cp2+1) % size;

					while (i != cp1 ) {
						short find_1 =-1;
						short find_2 =-1;

						int j = i;
						do {
							if (p1added.find(p2->at(j)) == p1added.end()) {
								find_1 = p2->at(j);
								p1added.insert(find_1);
								break;
							}
							j = (j+1) %size;
						} while (j != i) ;

						j = i;
						do {
							if (p2added.find(p1->at(j)) == p2added.end()) {
								find_2 = p1->at(j);
								p2added.insert(find_2);
								break;
							}
							j = (j+1) %size;
						} while (j != i);

						newGenotype1->set(i, find_1);
						newGenotype2->set(i, find_2);

						i = (i + 1) % size;

					}

				}

				// be careful individual create its own copy
				container->push_back(new Individual(newGenotype1, g));
				container->push_back(new Individual(newGenotype2, g));

			}

			return container;
		};

} // end namespace crossover

#endif /* OPERATORS_CROSSOVERCOLLECTION_H_ */
