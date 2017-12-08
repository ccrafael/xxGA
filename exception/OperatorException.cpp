/*
 * OperatorException.cpp
 *
 *  Created on: 6 Dec 2017
 *      Author: fali
 */

#include "OperatorException.h"

OperatorException::OperatorException() :
		std::runtime_error("Configuration problem.") {

}

OperatorException::OperatorException(std::string msg) :
		std::runtime_error(msg) {
	this->msg = msg;
}

std::string OperatorException::getMsg() {
	return this->msg;
}

void OperatorException::setMsg(std::string msg) {
	this->msg = msg;
}

OperatorException::~OperatorException() {
	// TODO Auto-generated destructor stub
}

char const * OperatorException::what() {
	return exception::what();
}

