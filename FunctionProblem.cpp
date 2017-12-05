#include "FunctionProblem.h"
using namespace std;

FunctionProblem::FunctionProblem() {

}

FunctionProblem::~FunctionProblem() {
}

FunctionProblem::FunctionProblem(string filename) {
	this->num_vars = 10;
	FunctionProblem::filename = filename;

	cout << "Reading problem file:  " << filename << "..." << endl;

	ifstream in(this->filename.data());

	if (!in) {
		cout << "Cannot open file." << endl;
		throw ProblemException(" Cannot open the config file. ");
	}

	string line;
	string key;
	string value;

	getline(in, line);
	int i = 0;

	while (in) {
		// eliminar el \r para textos windows
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
		} else if (line.length() > 0 && line.at(0) != '#'
				&& line.find("=") == string::npos) {
			//cout << "error at line: "<< i << " line: "<< line << endl;
		} else {
			//cout << "comentario o linea vacia: "<< line << endl;
		}
		getline(in, line);

		i++;
	}
	in.close();
}

int FunctionProblem::getInt(string pname) {
	int value;
	sscanf(properties[pname].data(), "%d", &value);
	return value;
}

double FunctionProblem::getDouble(string pname) {
	double value;
	sscanf(properties[pname].data(), "%lf", &value);
	return value;
}

string FunctionProblem::getProperty(string pname) {
	return properties[pname];
}

map<string, string> FunctionProblem::getPropertiesMap(void) {
	return properties;
}

double FunctionProblem::evaluate(Individual * individual) {
	return 0;
}

/*
 *
 *    	std::string function;
 std::string s;
 std::stringstream out;

 std::getline(std::cin, function);
 *
 *     if (num_vars > 0) {
 out << "x0";
 for (int i = 1; i < num_vars; i++) {
 out << ",x"<<i ;
 }
 s = out.str();
 } else {
 cout << "error numvars < 0" << endl;
 return -1;
 }

 fparser.AddConstant("pi", 3.1415926535897932);

 while(true)
 {
 std::cout << "Introduzca f("<< s <<"), ej: (x0*x1)/sqrt(x3) =? ";
 std::getline(std::cin, function);
 int res = fparser.Parse(function, s);
 if(res < 0) break;

 // parseamos la funcion
 std::cout << std::string(res+7, ' ') << "^\n"
 << fparser.ErrorMsg() << "\n\n";
 }

 *
 */

