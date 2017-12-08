#include "Config.h"

using namespace std;

log4cxx::LoggerPtr Config::logger(log4cxx::Logger::getLogger("config"));

Config::Config(string filename) {
	if (filename.empty()) {
		throw ConfigurationException("Configuration filename canat be empty.");
	}
	Config::filename = filename;

	LOG4CXX_DEBUG(logger, "Loading configuration = "<<filename);
	init();
}

string Config::find(string pname) {
	map<string, string>::iterator it = properties.find(pname);
	if (it != properties.end()) {
		return it->second;
	} else {
		throw NotFoundException("Unknown property: [" + pname + "]");
	}
}

int Config::getInt(string pname) {
	string svalue = find(pname);
	string::size_type sz;
	return stoi(svalue, &sz);

}

double Config::getDouble(string pname) {
	string svalue = find(pname);
	string::size_type sz;
	return stod(svalue, &sz);

}

string Config::getProperty(string pname) {
	return find(pname);
}

map<string, string> Config::getPropertiesMap(void) {
	return properties;
}

void Config::init() {

	ifstream in(this->filename.data());

	// check file is opened
	if (!in) {
		throw ConfigurationException(
				"Cannot open configuration file: " + filename);
	}

	string line;
	string key;
	string value;

	getline(in, line);
	int i = 0;

	// load the file
	while (in) {
		// remove \r for windows
		int l = line.length();
		line = l > 0 && line.at(l - 1) == '\r' ?
				line.substr(0, line.length() - 1) : line;

		if (line.length() > 0 && line.at(0) != '#'
				&& line.find("=") != string::npos) {
			vector<string> tokens;
			Util::tokenize(line, tokens, "=");
			key = tokens.size() > 0 ? Util::trim(tokens.at(0)) : "";
			value = tokens.size() > 1 ? Util::trim(tokens.at(1)) : "";
			this->properties[key] = value;
		}
		getline(in, line);

		i++;
	}
	in.close();
}

Config::~Config() {
}
