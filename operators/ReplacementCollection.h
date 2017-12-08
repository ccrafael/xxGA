/*
 * ReplacementCollection.h
 *
 *  Created on: 8 Dec 2017
 *      Author: fali
 */

#ifndef OPERATORS_REPLACEMENTCOLLECTION_H_
#define OPERATORS_REPLACEMENTCOLLECTION_H_

#include <functional>
#include "../IContainer.h"
#include "../Individual.h"
#include "../GenotypeBit.h"
#include "../exception/OperatorException.h"
#include "../Context.h"

namespace replacement {

/**
 * Do nothing.
 */
const std::function<IContainer*(IContainer*, IContainer*)> empty =
		[](IContainer* p, IContainer*) {
			return new IContainer();
		};

/**
 * Do nothing.
 */
const std::function<IContainer*(IContainer*, IContainer*)> basic =
		[](IContainer* p, IContainer*) {
			return new IContainer();
		};

}

#endif /* OPERATORS_REPLACEMENTCOLLECTION_H_ */
