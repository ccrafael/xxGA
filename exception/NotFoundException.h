/*
 * ConfigurationException.h
 *
 *  Created on: 6 Dec 2017
 *      Author: fali
 */

#ifndef EXCEPTION_NOTFOUND_EXCEPTION_H_
#define EXCEPTION_NOTFOUND_EXCEPTION_H_

#include <string>
#include <stdexcept>

class NotFoundException :  std::runtime_error {
	std::string msg;
public:
	NotFoundException();
	NotFoundException(std::string msg);

	virtual ~NotFoundException();

	std::string getMsg();
	void setMsg(std::string msg);

	virtual char const* what();
};

#endif /* EXCEPTION_NOTFOUND_EXCEPTION_H_ */
