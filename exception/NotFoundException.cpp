/*
 * ConfigurationException.cpp
 *
 *  Created on: 6 Dec 2017
 *      Author: fali
 */

#include "NotFoundException.h"

NotFoundException::NotFoundException() :
		std::runtime_error("Property not found.") {

}

NotFoundException::NotFoundException(std::string msg) :
		std::runtime_error(msg) {
	this->msg = msg;
}

std::string NotFoundException::getMsg() {
	return this->msg;
}

void NotFoundException::setMsg(std::string msg) {
	this->msg = msg;
}

NotFoundException::~NotFoundException() {
	// TODO Auto-generated destructor stub
}

char const * NotFoundException::what() {
	return exception::what();
}

