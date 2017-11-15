/*
 * Util.h
 *
 *  Created on: 11-dic-2008
 *      Author: rafael
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include <iostream>
#include <vector>
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
			gettimeofday(&time1,NULL);
			time = time1.tv_usec;
			int seed = time;
			srand(seed);
			already_init = true;
		}
	}

	static int rand(int r){
		Util::init();
		int random_number=  (std::rand () % r) ;
	    return random_number;
	}

	static double rand() {
		Util::init();
		return (double)std::rand() / (RAND_MAX+1.0);
	}

	static bool change(double rate) {
		return (Util::rand() <= rate);
	}

	static int b2i(vector<int> array, int offset, int length) {
		int valor = 0;
		int potencia = 1;
		for (int i = length+offset-1; i >= offset; i--) {
			valor += array[i]*potencia;
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
			for (int i = 0; i < vec.size(); i++) if (present = (vec[i]==r)) break;
			if (!present) {
				vec.push_back(r);
				j++;
			}
		}
		return vec;
	}

};


#endif /* UTIL_H_ */
