/*
 * Util.h
 *
 *  Created on: 11-dic-2008
 *      Author: rafael
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <stddef.h>
#include <sys/time.h>
#include <cstdlib>
#include <string>
#include <vector>

#include "GenotypeBit.h"

using namespace std;

class Util {
	static bool already_init;
public:
	Util();
	virtual ~Util();

	static void init() {
		if (!already_init) {
			struct timeval time1;
			long time;
			// inicializar semilla
			gettimeofday(&time1, NULL);
			time = time1.tv_usec;
			int seed = time;
			srand(seed);
			already_init = true;
		}
	}

	static int rand(int r) {
		Util::init();
		// TODO seed must be initialized and init should be called once
		int random_number = (std::rand() % r);
		return random_number;
	}

	static double rand() {
		Util::init();
		return (double) std::rand() / (RAND_MAX + 1.0);
	}

	static bool should_be_changed(double rate) {
		return (Util::rand() <= rate);
	}

	static int b2i(GenotypeBit * array, int offset, int length) {
		int valor = 0;
		int potencia = 1;
		for (int i = length + offset - 1; i >= offset; i--) {
			int bit = array->at(i)?1:0;
			valor += bit * potencia;
			potencia *= 2;
		}
		return valor;
	}

	/**
	 * obtiene size numeros aleatorios en el rango range sin
	 * repeticiones
	 */
	static vector<int> random(int range, int size) {
		vector<int> vec;
		int j = 0;
		bool present = false;
		while (j < size) {
			int r = Util::rand(range);
			// inline bucle
			for (unsigned int i = 0; i < vec.size(); i++) {
				present = (vec[i] == r);
				if (present) {
					break;
				}
			}
			if (!present) {
				vec.push_back(r);
				j++;
			}
		}
		return vec;
	}

	static string trim(string str) {
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

	static void tokenize(const string& str, vector<string>& tokens,
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

};

#endif /* UTIL_H_ */
