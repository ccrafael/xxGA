/*
 * SelectionCollection.h
 *
 * This header file contains a collection of selection operators. Each one is a
 * lambda expression.
 *
 *
 *  Created on: 8 Dec 2017
 *      Author: fali
 */

#ifndef OPERATORS_SELECTIONCOLLECTION_H_
#define OPERATORS_SELECTIONCOLLECTION_H_

#include <functional>
#include <stdexcept>
#include <algorithm>
#include "../IContainer.h"
#include "../Population.h"
#include "../Individual.h"
#include "../GenotypeBit.h"
#include "../exception/OperatorException.h"
#include "../Environment.h"
#include "../Util.h"

namespace selection {

/**
 * Do nothing.
 */
const std::function<IContainer* (Population*, unsigned int)> empty = [](Population* pop, unsigned int n) {
	return new IContainer();
};

/*
 * Select N individuals randomly from the population using tournament selection.
 *
 * Note population is ordered by fitness.
 *
 * TODO I think this method has no difference with the basic but that this one
 * allow repetition.
 */
const std::function<IContainer* (Population*, unsigned int)> tournament =
		[](Population* pop,unsigned  int n) {
			IContainer * container = new IContainer();
			Environment * context = Environment::instance();
			int pop_size = (int)pop->size();

			if (n > pop->size()) {
				throw invalid_argument("Pop size is smaller than tournament size");
			}

			while (container->size() < n) {
				vector<int> tournament = Util::random(pop_size, context->tournament_size);

				// the best is the higher number because population is ordered by fitness
				int selected = *std::max_element(tournament.begin(),tournament.end());

				container->push_back(pop->at(selected));

			}
			return container;
		};

/**
 * Select a set of context.num_parents parents randomly and without repetition.
 */
const std::function<IContainer* (Population*, unsigned  int)> basic =
		[](Population* pop,unsigned  int n) {
			IContainer * container = new IContainer();
			vector<int> selected = Util::random((int)pop->size(), n);
			for (unsigned int i = 0; i < selected.size(); i++) {
				container->push_back(pop->at(selected.at(i)));
			}

			return container;
		};

const std::function<IContainer* (Population*, unsigned  int)> basic_migration =
		[](Population* pop,unsigned  int n) {
			IContainer * container = new IContainer();
			Environment * context = Environment::instance();

			// select without repetition
			vector<int> selected = Util::random((int)pop->size(), n);
			for (unsigned int i = 0; i < selected.size(); i++) {
				container->push_back(pop->at(selected.at(i)));
			}

			return container;
		};

const std::function<IContainer* (Population*,unsigned  int)> worsts_migration =
		[](Population* pop,unsigned  int n) {
			return pop->worsts(n);
		};

const std::function<IContainer* (Population*,unsigned  int)> bests_migration =
		[](Population* pop,unsigned  int n) {
			return pop->bests(n);
		};

const std::function<IContainer* (Population*,unsigned  int)> fitnessproportional =
		[](Population* pop,unsigned  int n) {
			throw runtime_error("fitnessproportional not implemented yet");
			return nullptr;
		};

const std::function<IContainer* (Population*,unsigned  int)> stochasticuniversalsampling =
		[](Population* pop,unsigned  int n) {
			throw runtime_error("stochasticuniversalsampling not implemented yet");
			return nullptr;
		};

const std::function<IContainer* (Population*,unsigned  int)> ranking =
		[](Population* pop,unsigned  int n) {
			throw runtime_error("ranking not implemented yet");
			return nullptr;
		};

} // end namespace selection

#endif /* OPERATORS_SELECTIONCOLLECTION_H_ */
