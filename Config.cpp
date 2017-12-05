#include "Config.h"

using namespace std;

Config::Config() {
	init();
}


Config::Config(string filename) {
	Config::filename = filename;
	cout << filename << endl;
	init();
}

int Config::getInt(string pname) {
	int value;
	sscanf(properties[pname].data(), "%d", &value);
	return value;
}

double Config::getDouble(string pname) {
	double value;
	sscanf(properties[pname].data(), "%lf", &value);
	return value;
}

string Config::getProperty(string pname) {
	return properties[pname];
}

map<string, string> Config::getPropertiesMap(void) {
	return properties;
}

void Config::init() {
	cout <<"reading file:  "<< filename << "..." << endl;

	ifstream in( this->filename.data() );

	if(!in) {
		cout << "Cannot open file."<<endl;
		return;
	}

	string line;
	string key;
	string value;

	getline(in, line);
	int i =0;

	while (in) {
		// eliminar el \r para textos windows
		int l = line.length();
		line = l > 0 && line.at(l-1) == '\r'?line.substr(0,line.length()-1):line;

		if(line.length() > 0 && line.at(0) != '#' && line.find("=")!=string::npos ) {
			vector<string> tokens;
			Util::tokenize(line, tokens, "=");
			key = tokens.size() > 0 ? Util::trim(tokens.at(0)):"";
			value = tokens.size() > 1 ? Util::trim(tokens.at(1)):"";
			this->properties[key] = value;
		} else if ( line.length() > 0 && line.at(0) != '#' && line.find("=")==string::npos){
			//cout << "error at line: "<< i << " line: "<< line << endl;
		} else {
			//cout << "comentario o linea vacia: "<< line << endl;
		}
		getline(in, line);

		i++;
	  }
	  in.close();
}
Config::~Config() {
}
