/*
 * ConfigurationException.h
 *
 *  Created on: 6 Dec 2017
 *      Author: fali
 */

#ifndef EXCEPTION_CONFIGURATIONEXCEPTION_H_
#define EXCEPTION_CONFIGURATIONEXCEPTION_H_

#include <string>
#include <stdexcept>

class ConfigurationException :  std::runtime_error {
	std::string msg;
public:
	ConfigurationException();
	ConfigurationException(std::string msg);

	virtual ~ConfigurationException();
	std::string getMsg();
	void setMsg(std::string msg);

	virtual char const* what();
};

#endif /* EXCEPTION_CONFIGURATIONEXCEPTION_H_ */
