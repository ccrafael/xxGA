/*
 * Individual_test.h
 *
 *  Created on: 9 Dec 2017
 *      Author: fali
 */

#ifndef UTEST_FUNCTIONSPROBLEM_TEST_H_
#define UTEST_FUNCTIONSPROBLEM_TEST_H_

#include <sstream>
#include <iostream>
#include "utests.h"
#include "../Individual.h"
#include "../GenotypeBit.h"
#include "../problems/FunctionsProblem.h"

/*
 * A unit test for Individual and genotypebit.
 */

class Functions_test: public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(Functions_test);
	CPPUNIT_TEST(test);

	CPPUNIT_TEST_SUITE_END()
	;

public:
	void setUp(void);
	void tearDown(void);

protected:
	void test(void);

private:
	FunctionsProblem *testObj;
	Config * config;

};

void Functions_test::test(void) {
	GenotypeBit * genotype = new GenotypeBit();
	genotype->push_back(0);
	genotype->push_back(0);
	genotype->push_back(0);
	genotype->push_back(0);
	genotype->push_back(0); //-16

	genotype->push_back(0);
	genotype->push_back(0);
	genotype->push_back(0);
	genotype->push_back(0);
	genotype->push_back(0); //-16

	genotype->binaryToGray();
	Individual * individual = new Individual(genotype, 0);

	cout << "----->" << individual << testObj->decode(individual) << endl;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0007806, testObj->evaluate(individual),
			0.0000001);
	delete individual;
	delete genotype;

	genotype = new GenotypeBit();
	genotype->push_back(1);
	genotype->push_back(1);
	genotype->push_back(1);
	genotype->push_back(1);
	genotype->push_back(1); //15

	genotype->push_back(1);
	genotype->push_back(1);
	genotype->push_back(1);
	genotype->push_back(1);
	genotype->push_back(1); //15

	genotype->binaryToGray();
	individual = new Individual(genotype, 0);

	cout << "----->" << individual << testObj->decode(individual) << endl;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0008880994, testObj->evaluate(individual),
			0.0000001);
	delete individual;
	delete genotype;

	genotype = new GenotypeBit();
	genotype->push_back(0);
	genotype->push_back(0);
	genotype->push_back(0);
	genotype->push_back(0);
	genotype->push_back(0); //-16

	genotype->push_back(1);
	genotype->push_back(1);
	genotype->push_back(1);
	genotype->push_back(1);
	genotype->push_back(1); //15

	genotype->binaryToGray();
	individual = new Individual(genotype, 0);

	cout << "----->" << individual << testObj->decode(individual) << endl;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0038759689, testObj->evaluate(individual),
			0.0000001);
	delete individual;
	delete genotype;

	genotype = new GenotypeBit();
	genotype->push_back(0);
	genotype->push_back(0);
	genotype->push_back(0);
	genotype->push_back(1);
	genotype->push_back(1); //-13

	genotype->push_back(0);
	genotype->push_back(1);
	genotype->push_back(1);
	genotype->push_back(1);
	genotype->push_back(1); // -1

	genotype->binaryToGray();
	individual = new Individual(genotype, 0);

	cout << "----->" << individual << testObj->decode(individual) << endl;
	CPPUNIT_ASSERT_DOUBLES_EQUAL(0.0027322404, testObj->evaluate(individual),
			0.0000001);
	delete individual;
	delete genotype;
}

void Functions_test::setUp(void) {
	config = new Config(string("utest/config_functions"));
	testObj = new FunctionsProblem(config);

}

void Functions_test::tearDown(void) {
	delete testObj;
	delete config;
}

#endif /* UTEST_FUNCTIONSPROBLEM_TEST_H_ */
