/*
 * IContainer.h
 *
 *  Created on: 26 Nov 2017
 *      Author: fali
 */

#ifndef ICONTAINER_H_
#define ICONTAINER_H_

#include <vector>
#include <iterator>
#include <algorithm>
#include "Individual.h"

/*
 * Just a collection of individuals. It is really used to decouple the implementation
 * from the differents collections from the std library.
 *
 * That way the sets returned by an operator are not mandatory to be ordered or
 * unordered.
 *
 * TODO should be a template
 */
typedef vector<Individual*> IContainer;

#endif /* ICONTAINER_H_ */
