/*
 * ConfigurationException.cpp
 *
 *  Created on: 6 Dec 2017
 *      Author: fali
 */

#include "ConfigurationException.h"

ConfigurationException::ConfigurationException() :
		std::runtime_error("Configuration problem.") {

}

ConfigurationException::ConfigurationException(std::string msg) :
		std::runtime_error(msg) {
	this->msg = msg;
}

std::string ConfigurationException::getMsg() {
	return this->msg;
}

void ConfigurationException::setMsg(std::string msg) {
	this->msg = msg;
}

ConfigurationException::~ConfigurationException() {
	// TODO Auto-generated destructor stub
}

char const * ConfigurationException::what() {
	return exception::what();
}

