#include "Config.h"

using namespace std;

string trim( string str)
{
	// Trim Both leading and trailing spaces
	size_t startpos = str.find_first_not_of(" \t"); // Find the first character position after excluding leading blank spaces
	size_t endpos = str.find_last_not_of(" \t"); // Find the first character position from reverse af

	// if all spaces or empty return an empty string
	if( string::npos == startpos  ||  string::npos == endpos) {
		return "";
	} else {
		return str.substr( startpos, endpos-startpos+1 );
	}
}

void tokenize(const string& str,
	vector<string>& tokens,
	const string& delimiters = " ")
{
    // Skip delimiters at beginning.
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);

    // Find first "non-delimiter".
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}

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
			tokenize(line, tokens, "=");
			key = tokens.size() > 0?trim(tokens.at(0)):"";
			value = tokens.size() > 1?trim(tokens.at(1)):"";
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
