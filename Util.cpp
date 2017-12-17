/*
 * Util.cpp
 *
 *  Created on: 11-dic-2008
 *      Author: rafael
 */

#include "Util.h"

using namespace std;

bool Util::already_init = false;

double Util::rand() {
	thread_local std::random_device rd; //Will be used to obtain a seed for the random number engine
	std::mt19937 generator(rd());
	std::uniform_real_distribution<double> dist(0, 1);

	return dist(generator);
}

int Util::b2i(vector<bool> array, int offset, int length) {
	int valor = 0;
	int potencia = 1;
	for (int i = length + offset - 1; i >= offset; i--) {
		int bit = array[i] ? 1 : 0;
		valor += bit * potencia;
		potencia *= 2;
	}
	return valor;
}

bool Util::should_be_changed(double rate) {
	return (Util::rand() <= rate);
}

vector<int> Util::random(int range, int size) {
	vector<int> vec;
	int j = 0;
	bool present = false;

	thread_local std::random_device rd; //Will be used to obtain a seed for the random number engine
	std::mt19937 generator(rd());
	std::uniform_int_distribution<int> dis(0, range - 1);

	while (j < size) {
		int r = dis(generator);

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

int Util::rand(int r) {
	// need to be created every time or there will be problems with the
	// random generations
	thread_local std::random_device rd; //Will be used to obtain a seed for the random number engine

	std::mt19937 generator(rd());
	std::uniform_int_distribution<int> dis(0, r - 1);

	return dis(generator);
}
