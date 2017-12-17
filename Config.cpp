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

void tokenize(const string& str, vector<string>& tokens,
			const string& delimiters = " ") {
		// Skip delimiters at beginning.
		string::size_type lastPos = str.find_first_not_of(delimiters, 0);

		// Find first "non-delimiter".
		string::size_type pos = str.find_first_of(delimiters, lastPos);

		while (string::npos != pos || string::npos != lastPos) {
			// Found a token, add it to the vector.
			tokens.push_back(str.substr(lastPos, pos - lastPos));
			// Skip delimiters.  Note the "not_of"
			lastPos = str.find_first_not_of(delimiters, pos);
			// Find next "non-delimiter"
			pos = str.find_first_of(delimiters, lastPos);
		}
	}

string trim(string str) {
		// Trim Both leading and trailing spaces
		size_t startpos = str.find_first_not_of(" \t"); // Find the first character position after excluding leading blank spaces
		size_t endpos = str.find_last_not_of(" \t"); // Find the first character position from reverse af

		// if all spaces or empty return an empty string
		if (string::npos == startpos || string::npos == endpos) {
			return "";
		} else {
			return str.substr(startpos, endpos - startpos + 1);
		}
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
			tokenize(line, tokens, "=");
			key = tokens.size() > 0 ? trim(tokens.at(0)) : "";
			value = tokens.size() > 1 ? trim(tokens.at(1)) : "";
			this->properties[key] = value;
		}
		getline(in, line);

		i++;
	}
	in.close();
}

Config::~Config() {
}
