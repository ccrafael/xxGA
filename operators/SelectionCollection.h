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
#include "../Context.h"
#include "../Util.h"

namespace selection {

/**
 * Do nothing.
 */
const std::function<IContainer* (Population*)> empty = [](Population* pop) {
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
const std::function<IContainer* (Population*)> tournament =
		[](Population* pop) {
			IContainer * container = new IContainer();
			Context * context = Context::instance();

			while (container->size() < context->num_parents) {
				vector<int> tournament = Util::random((int)pop->size(), context->tournament_size);

				// the best is the biggest because population is ordered by fitness
				int selected = *std::max_element(tournament.begin(),tournament.end());

				container->push_back(pop->at(selected));

			}
			return container;
		};

/**
 * Select a set of context.num_parents parents randomly and without repetition.
 */
const std::function<IContainer* (Population*)> basic = [](Population* pop) {
	IContainer * container = new IContainer();
	Context * context = Context::instance();

	vector<int> selected = Util::random((int)pop->size(), context->num_parents);
	for (unsigned int i = 0; i < selected.size(); i++) {
		container->push_back(pop->at(selected.at(i)));
	}

	return container;
};

const std::function<IContainer* (Population*)> basicmigration =
		[](Population* pop) {
			IContainer * container = new IContainer();
			Context * context = Context::instance();

			vector<int> selected = Util::random((int)pop->size(), context->num_migrants);
			for (unsigned int i = 0; i < selected.size(); i++) {
				container->push_back(pop->at(selected.at(i)));
			}

			return container;
		};

const std::function<IContainer* (Population*)> worsts = [](Population* pop) {
	Context * context = Context::instance();
	return pop->worsts(context->num_migrants);
};

const std::function<IContainer* (Population*)> fitnessproportional =
		[](Population* pop) {
			throw runtime_error("fitnessproportional not implemented yet");
			return nullptr;
		};

const std::function<IContainer* (Population*)> stochasticuniversalsampling =
		[](Population* pop) {
			throw runtime_error("stochasticuniversalsampling not implemented yet");
			return nullptr;
		};

const std::function<IContainer* (Population*)> ranking = [](Population* pop) {
	throw runtime_error("ranking not implemented yet");
	return nullptr;
};

} // end namespace selection

#endif /* OPERATORS_SELECTIONCOLLECTION_H_ */
