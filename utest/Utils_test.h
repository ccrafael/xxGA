/*
 * Individual_test.h
 *
 *  Created on: 9 Dec 2017
 *      Author: fali
 */

#ifndef UTEST_UTILPROBLEM_TEST_H_
#define UTEST_UTILPROBLEM_TEST_H_

#include <sstream>
#include <iostream>
#include "utests.h"
#include "../Util.h"

/*
 * A unit test for Individual and genotypebit.
 */

class Util_test: public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(Util_test);
	CPPUNIT_TEST(test);

	CPPUNIT_TEST_SUITE_END()
	;

public:
	void setUp(void);
	void tearDown(void);

protected:
	void test(void);

private:

};

void Util_test::test(void) {
	long mutate = 0;
	long notmutate = 0;

	// test the random generation
	for (int i = 0; i < 10000000; i++) {
		if (Util::should_be_changed(0.07)) {
			mutate++;
		} else {
			notmutate++;
		}
	}
	cout << "mutate: " << mutate << endl;
	cout << "notmutate: " << notmutate << endl;

	cout << "rate: " << mutate / double(mutate + notmutate) << endl;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.07, mutate / double(mutate + notmutate),
			0.0001);

	for (int i = 0; i < 1000000; i++) {
		int v = Util::rand(2);
		CPPUNIT_ASSERT(v == 1 || v == 0);
	}

	for (int i = 0; i < 1000000; i++) {
		vector<int> v = Util::random(2, 2);
		CPPUNIT_ASSERT(v.size() == 2);
		CPPUNIT_ASSERT((v[0] == 1 || v[0] == 0) && (v[1] == 1 || v[1] == 0));
	}

}

void Util_test::setUp(void) {

}

void Util_test::tearDown(void) {

}

#endif /* UTEST_FUNCTIONSPROBLEM_TEST_H_ */
