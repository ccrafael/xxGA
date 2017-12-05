/*
 * Operator.h
 *
 *  Created on: 25 Nov 2017
 *      Author: fali
 */

#ifndef OPERATOR_H_
#define OPERATOR_H_

#include "IContainer.h"
#include "Individual.h"

/*
 * This class represent a generic operator to do any operation to a set of
 * individuals. Like mutation, selection or crossover.
 */
class Operator {
public:
	/*
	 * Do the operator action.
	 * @param indivuduals The set of input individuals.
	 * @return The set of output individuals.
	 */
	virtual IContainer * operate(IContainer * individuals) = 0;
	virtual ~Operator() {};
};

#endif /* OPERATOR_H_ */
