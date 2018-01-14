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
	thread_local std::random_device rd;
	thread_local std::mt19937 generator(rd());
	thread_local std::uniform_real_distribution<double> dist(0, 1);

	return dist(generator);
}

unsigned long Util::b2i(vector<bool> array, int offset, int length) {
	unsigned long valor = 0;
	unsigned long potencia = 1;

	for (int i = length + offset - 1; i >= offset; i--) {
		valor += array[i] * potencia;
		potencia *= 2;
	}
	return valor;
}

bool Util::should_be_changed(double rate) {
	return (Util::rand() <= rate);
}

vector<int> Util::random(int range, int size) {

	unordered_set<int> s;

	thread_local std::random_device rd;
	thread_local std::mt19937 generator(rd());
	std::uniform_int_distribution<int> dis(0, range - 1);

	while (s.size() < (unsigned int) size) {
		int r = dis(generator);
		s.insert(r);
	}

	vector<int> vec(s.begin(), s.end());
	return vec;
}

int Util::rand(int r) {
	// need to be created every time or there will be problems with the
	// random generations
	thread_local std::random_device rd; //Will be used to obtain a seed for the random number engine
	thread_local std::mt19937 generator(rd());
	std::uniform_int_distribution<int> dis(0, r - 1);

	return dis(generator);
}
