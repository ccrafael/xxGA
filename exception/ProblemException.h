/*
 * ConfigException.h
 *
 *  Created on: 27-ene-2009
 *      Author: rafael
 */

#ifndef PROBLEMEXCEPTION_H_
#define PROBLEMEXCEPTION_H_

using namespace std;

class ProblemException {
	const char * msg;
public:
	ProblemException(const char * msg);
	virtual ~ProblemException();
};

#endif /* PROBLEMEXCEPTION_H_ */
