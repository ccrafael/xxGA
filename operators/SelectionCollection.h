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
#include "../IContainer.h"
#include "../Individual.h"
#include "../GenotypeBit.h"
#include "../exception/OperatorException.h"

namespace selection {

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


			return new IContainer();
		};
}

#endif /* OPERATORS_SELECTIONCOLLECTION_H_ */
