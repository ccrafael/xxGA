/*
 * OperatorException.h
 *
 *  Created on: 6 Dec 2017
 *      Author: fali
 */

#ifndef EXCEPTION_OperatorException_H_
#define EXCEPTION_OperatorException_H_

#include <string>
#include <stdexcept>

class OperatorException :  std::runtime_error {
	std::string msg;
public:
	OperatorException();
	OperatorException(std::string msg);

	virtual ~OperatorException();
	std::string getMsg();
	void setMsg(std::string msg);

	virtual char const* what();
};

#endif /* EXCEPTION_OperatorException_H_ */
