/*
 * ReplacementCollection.h
 *
 *  Created on: 8 Dec 2017
 *      Author: fali
 */

#ifndef OPERATORS_REPLACEMENTCOLLECTION_H_
#define OPERATORS_REPLACEMENTCOLLECTION_H_

#include <functional>
#include <stdexcept>
#include "../IContainer.h"
#include "../Individual.h"
#include "../Population.h"
#include "../GenotypeBit.h"
#include "../exception/OperatorException.h"
#include "../Context.h"

namespace replacement {

/**
 * Do nothing.
 */
const std::function<IContainer*(Population*, IContainer*)> empty =
		[](Population* p, IContainer*) {
			return new IContainer();
		};

/**
 * Do nothing.
 */
const std::function<IContainer*(Population*, IContainer*)> generational =
		[](Population* p, IContainer*) {
			throw runtime_error("generational model not implemented yet");
			return nullptr;
		};

const std::function<IContainer*(Population*, IContainer*)> steadystate =
		[](Population* p, IContainer* group) {

			// the get the worst by fitness
			IContainer* toremove= p->worsts(group->size());

			// just add the new  individuals
			p->add(group);

			return toremove;
		};

} // end namespace replacement

#endif /* OPERATORS_REPLACEMENTCOLLECTION_H_ */
