/*
 * Config.h
 *
 *  Created on: 29-nov-2008
 *      Author: rafael
 */
#ifndef CONFIG_H_
#define CONFIG_H_

#include "sys.h"
#include "Util.h"

using namespace std;

class Config {
	map<string, string> properties;
	string filename;

public:
	Config();

	Config(string filename);
	virtual ~Config();

	string getProperty(string pname);
	int getInt(string pname);
	double getDouble(string pname);
	map<string, string> getPropertiesMap();
private:
	void init();
};

#endif /* CONFIG_H_ */

