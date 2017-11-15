/*
 * test.cpp
 *
 *  Created on: 10-dic-2008
 *      Author: rafael
 */
#include "../Util.h"
#include "../ga.h"
#include "../Environment.h"
#include "../fparser/fparser.hh"
#include "../Phenotype.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <math.h>

struct variable {
	double min;
	double max;
	int bits;
	double step;
	double count;
};

// variables globales
int num_vars;

// se usa para evaluar la funcion buscando en todo el dominio
variable configvar[100];

FunctionParser fparser;

int main( int argc, char** argv ) {
	fparser.AddConstant("pi", 3.1415926535897932);

    std::string function;

    std::cout << " Introduzca N de f(x1,...,xN): ";
    std::cin >> num_vars;

    std::string s;
    std::stringstream out;

    if (num_vars > 0) {
	    out << "x0";
	    for (int i = 1; i < num_vars; i++) {
	    	out << ",x"<<i ;
	    }
	    s = out.str();
    } else {
    	cout << "error numvars < 0" << endl;
    	return -1;
    }

    std::getline(std::cin, function);
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

    // se usa para crear el fenotipo
    vector<var*> vars;

    int genes = 0;
    double min, max, numbits;

    cout << " Vars min value: ";
    cin >> min;
    cout << " Vars max value: ";
    cin >> max;
    cout << " Vars num bits: ";
    cin >> numbits;

    for (int i = 0; i < num_vars; i++) {
    	var *v = new var();

    	configvar[i].min = min;
    	configvar[i].max = max;
    	configvar[i].bits = numbits;

    	configvar[i].step = (configvar[i].max - configvar[i].min)/ pow(2, configvar[i].bits) ;
    	configvar[i].count = configvar[i].min;

    	v->min = configvar[i].min;
    	v->max = configvar[i].max;
    	v->bits = configvar[i].bits;

    	vars.push_back(v);

    	genes +=  configvar[i].bits;
    }

    Phenotype * ph = new Phenotype(vars, function, s);
    GA * ga = new GA(ph);

    GA::GenesNumber = genes;

    ga->run();

    return 0;
}
