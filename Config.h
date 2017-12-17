/*
 * Config.h
 *
 *  Created on: 29-nov-2008
 *      Author: rafael
 */
#ifndef CONFIG_H_
#define CONFIG_H_

#include <iostream>
#include <fstream>

#include "log4cxx/logger.h"
#include "exception/ConfigurationException.h"
#include "exception/NotFoundException.h"

using namespace std;

/*
 * Configuration file.
 * TODO should be better  a singleton?
 */
class Config {

	static log4cxx::LoggerPtr logger;

	map<string, string> properties;
	string filename;

public:
	Config(string filename);
	virtual ~Config();

	string getProperty(string pname);
	int getInt(string pname);
	/*
	 * Get a dobule value of that property.
	 * TODO what happend if the property is not a double.
	 * @param pname A property name.
	 * @return
	 */
	double getDouble(string pname);
	/*
	 * Find a property.
	 * @param pname A property name.
	 * @return The value of the property if found, if not
	 * an NotFoundException is thrown.
	 */
	string find(string pname);
	/*
	 * Get the properties map.
	 * @return The properties map.
	 */
	map<string, string> getPropertiesMap();
private:
	void init();
};

#endif /* CONFIG_H_ */

