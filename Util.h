/*
 * Util.h
 *
 * TODO refactor all random generations the cost to get a random this way
 *  is currently quite high.
 *
 *  Created on: 11-dic-2008
 *      Author: rafael
 */

#ifndef UTIL_H_
#define UTIL_H_

#include <random>
#include <unordered_set>
#include <set>
#include <string>
#include <vector>


using namespace std;

/*
 * The classic evil collection of static methods.
 *
 * TODO refactor this, separated random generation.
 */
class Util {
	static bool already_init;
	//static std::map<char, > random_devices_pool;
public:

	/*
	 * Generate a uniform random int between [0, r)
	 */
	static int rand(int r) ;

	/*
	 * Generate randoms between 0,1;
	 */
	static double rand();


	static bool should_be_changed(double rate);

	static unsigned long b2i(vector<bool> array, int offset, int length) ;

	/**
	 * Get random numbers in the range [0, range) without repetitions.
	 * return a vector of random numbers in the range [0, range) without repetitions.
	 */
	static vector<int> random(int range, int size);


};

#endif /* UTIL_H_ */
