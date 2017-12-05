/*
 * EmptyOperator.h
 *
 *  Created on: 5 Dec 2017
 *      Author: fali
 */

#ifndef OPERATORS_EMPTYOPERATOR_H_
#define OPERATORS_EMPTYOPERATOR_H_

#include "../Operator.h"

class EmptyOperator : public Operator {
public:
	EmptyOperator();
	virtual ~EmptyOperator();

	virtual IContainer * operate(IContainer * individuals) override;
};

#endif /* OPERATORS_EMPTYOPERATOR_H_ */
