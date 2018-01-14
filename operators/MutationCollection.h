/*
 * MutationCollection.h
 *
 *  Created on: 8 Dec 2017
 *      Author: fali
 */

#ifndef OPERATORS_MUTATIONCOLLECTION_H_
#define OPERATORS_MUTATIONCOLLECTION_H_

#include <functional>
#include <random>
#include "../IContainer.h"
#include "../Individual.h"
#include "../GenotypeBit.h"
#include "../GenotypeNumber.h"
#include "../exception/OperatorException.h"
#include "../Environment.h"

namespace mutation {

/**
 * Do nothing.
 */
const std::function<void(IContainer*)> empty = [](IContainer* i) {
	// do what was promised
	};

/**
 * Do nothing.
 */
const std::function<void(IContainer*)> rate = [](IContainer* g) {
	Environment* context = Environment::instance();

	// cost to create this is high so is better create it here
	// instead calling Util
		thread_local std::random_device rd;
		std::mt19937 generator(rd());
		std::uniform_real_distribution<double> dist(0, 1);

		std::for_each(g->begin(), g->end(),
				[&generator, &dist, context](Individual * i) {

					GenotypeBit * gn = dynamic_cast<GenotypeBit*>( i->get_genotype());
					if (gn == nullptr) {
						throw OperatorException("Bad casting");
					}
					for (int i = 0; i < gn->size(); i++) {
						if ( dist(generator) <= context->mutation_rate ) {
							gn->set(i, !gn->at(i));
						}
					}
				});

	};

const std::function<void(IContainer*)> basic =
		[](IContainer* g) {
			Environment* context = Environment::instance();

			std::for_each(g->begin(), g->end(),
					[context](Individual * i) {

						if (Util::should_be_changed(context->mutation_prop)) {
							GenotypeBit * gn = dynamic_cast<GenotypeBit*>(i->get_genotype());
							if (gn == nullptr) {
								throw OperatorException("Bad casting");
							}
							int i = Util::rand(gn->size());
							gn->set(i, !gn->at(i));
						}});
		};

const std::function<void(IContainer*)> shortpermutation =
		[](IContainer* g) {

			double p = Environment::instance()->mutation_prop;
			std::for_each(g->begin(), g->end(),
					[p](Individual * i) {

						if (Util::should_be_changed(p)) {
							GenotypeNumber<short> * gn = dynamic_cast<GenotypeNumber<short>*>(i->get_genotype());
							if (gn == nullptr) {
								throw OperatorException("Bad casting");
							}
							int pos = Util::rand(gn->size()-1);

							short aux = gn->at(pos);
							gn->set(pos, gn->at(pos+1));
							gn->set(pos+1, aux);
						}

					});

		};

const std::function<void(IContainer*)> permutation =
		[](IContainer* g) {

			double p = Environment::instance()->mutation_prop;
			std::for_each(g->begin(), g->end(),
					[p](Individual * i) {

						if (Util::should_be_changed(p)) {
							GenotypeNumber<short> * gn = dynamic_cast<GenotypeNumber<short>*>(i->get_genotype());
							if (gn == nullptr) {
								throw OperatorException("Bad casting");
							}
							vector<int> pos = Util::random(gn->size(), 2);

							short aux = gn->at(pos[0]);
							gn->set(pos[0], gn->at(pos[1]));
							gn->set(pos[1], aux);
						}

					});

		};

const std::function<void(IContainer*)> insertion =
		[](IContainer* g) {

			double p = Environment::instance()->mutation_prop;

			std::for_each(g->begin(), g->end(),
					[p](Individual * i) {

						if (Util::should_be_changed(p)) {
							GenotypeNumber<short> * gn = dynamic_cast<GenotypeNumber<short>*>(i->get_genotype());
							if (gn == nullptr) {
								throw OperatorException("Bad casting");
							}
							vector<int> pos = Util::random(gn->size(), 2);

							int max_p = max(pos[0], pos[1]);
							int min_p = min(pos[0], pos[1]);

							short v = gn->at(max_p);
							for (int i = max_p; i > min_p; i--) {
								gn->set(i, gn->at(i-1));
							}

							gn->set(min_p, v);
						}

					});

		};

} // end

#endif /* OPERATORS_MUTATIONCOLLECTION_H_ */
