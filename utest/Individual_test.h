/*
 * Individual_test.h
 *
 *  Created on: 9 Dec 2017
 *      Author: fali
 */

#ifndef UTEST_INDIVIDUAL_TEST_H_
#define UTEST_INDIVIDUAL_TEST_H_

#include <sstream>
#include <iostream>
#include "utests.h"
#include "../Individual.h"
#include "../GenotypeBit.h"

/*
 * A unit test for Individual and genotypebit.
 */

class Individual_test: public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(Individual_test);
	CPPUNIT_TEST(test);

	CPPUNIT_TEST_SUITE_END()
	;

public:
	void setUp(void);
	void tearDown(void);

protected:
	void test(void);

private:
	Individual *testObj;
	Individual *testObj2;
};


void Individual_test::test(void) {
	GenotypeBit * g = (GenotypeBit*)testObj->get_genotype();
	CPPUNIT_ASSERT(nullptr != g);
	CPPUNIT_ASSERT(g->size() == 5);
	CPPUNIT_ASSERT(testObj->fitness() == 0);
	testObj->fitness(7);
	CPPUNIT_ASSERT(testObj->fitness() == 7);
	CPPUNIT_ASSERT(testObj->birth() == 3);
	CPPUNIT_ASSERT(testObj->isEvaluated() == false);
	testObj->setEvaluated(true);
	CPPUNIT_ASSERT(testObj->isEvaluated() == true);

	std::ostringstream s;
	s << testObj2;
	std::cout << s.str() << endl;
	CPPUNIT_ASSERT(0 == s.str().compare("1110110 ind not evaluated"));
	testObj2->fitness(1);

	s.clear();
	s.str("");
	s << testObj2;
	std::cout << s.str() << endl;
	CPPUNIT_ASSERT(0 == s.str().compare("1110110 ind not evaluated"));

	testObj2->setEvaluated(true);

	s.clear();
	s.str("");
	s << testObj2;
	std::cout << s.str() << endl;
	CPPUNIT_ASSERT(0 == s.str().compare("1110110 - fitness: 1"));
}

void Individual_test::setUp(void) {
	GenotypeBit * g = new GenotypeBit(3);
	testObj = new Individual(g, 3);

	g = new GenotypeBit();
	g->push_back(true);
	g->push_back(true);
	g->push_back(true);
	g->push_back(false);
	g->push_back(true);
	g->push_back(true);
	g->push_back(false);
	testObj2 = new Individual(g, 3);
}

void Individual_test::tearDown(void) {
	delete testObj;
}

#endif /* UTEST_INDIVIDUAL_TEST_H_ */
