/*
 * Config.h
 *
 *  Created on: 29-nov-2008
 *      Author: rafael
 */
#ifndef PROBLEM_H_
#define PROBLEM_H_

#include "Individual.h"

using namespace std;

/*
 * Interface that represent a problem. Basically it is used to decode, the posible
 * solutions of the problem and also to evaluate those solutions.
 */
class Problem {
public:
	virtual double evaluate(Individual * individual) =  0;
	virtual ~Problem() {};
};

#endif /* PROBLEM_H_ */

