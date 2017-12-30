/*
 * Individual_test.h
 *
 *  Created on: 9 Dec 2017
 *      Author: fali
 */

#ifndef UTEST_POPULATION_TEST_H_
#define UTEST_POPULATION_TEST_H_

#include <sstream>
#include <iostream>
#include "utests.h"
#include "../Population.h"
#include "../GenotypeBit.h"
#include "../ga.h"
#include "../problems/ParseFunctionProblem.h"

/*
 * A unit test for Population and all its dependencies, therefore
 * config and function prolem are somehow tested.
 */

class Population_test: public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(Population_test);
	CPPUNIT_TEST(test);

	CPPUNIT_TEST_SUITE_END()
	;

public:
	void setUp(void);
	void tearDown(void);

protected:
	void test(void);

private:
	Population * pop;
	Problem * problem;
	Config * config;
};

void Population_test::test(void) {

	CPPUNIT_ASSERT(nullptr != pop);
	CPPUNIT_ASSERT(10 == pop->size());

	IContainer * ind = pop->get_individuals();
	CPPUNIT_ASSERT(10 == ind->size());

	Individual * worst = pop->worst();
	Individual * best = pop->best();

	std::for_each(ind->begin(), ind->end(), [this](Individual * i) {
		cout << i << " "<< problem->decode(i)<<endl;

		CPPUNIT_ASSERT(i->isEvaluated() == true);
		CPPUNIT_ASSERT(i->birth() == 0);

	});
	CPPUNIT_ASSERT(worst->fitness() <= best->fitness());

	CPPUNIT_ASSERT_EQUAL(worst, pop->at(0));
	CPPUNIT_ASSERT_EQUAL(best, pop->at(9));

	delete ind;

	IContainer * worsts = pop->worsts(2);

	CPPUNIT_ASSERT_EQUAL((int )worsts->size(), 2);
	CPPUNIT_ASSERT_EQUAL(worsts->at(0), pop->at(0));
	CPPUNIT_ASSERT_EQUAL(worsts->at(1), pop->at(1));

	// test evaluation
	GenotypeBit * genotype = new GenotypeBit();
	genotype->push_back(true);
	genotype->push_back(true);
	genotype->push_back(true);
	genotype->push_back(true);
	genotype->push_back(true);

	genotype->push_back(true);
	genotype->push_back(true);
	genotype->push_back(true);
	genotype->push_back(true);
	genotype->push_back(true);

	cout << " genotype binary: " << genotype << endl;
	// flip the genotype to gray
	genotype->binaryToGray();

	cout << " genotype gray:   " << genotype << endl;

	vector<bool> g = genotype->grayToBinary();
	std::for_each(g.begin(), g.end(), [](bool b) {cout << (b?"1":"0");});
	Individual * i = new Individual(genotype, 0);

	CPPUNIT_ASSERT_DOUBLES_EQUAL(problem->evaluate(i), 0.00513479, 0.0000001);

	delete i;
	delete genotype;

	genotype = new GenotypeBit();
	genotype->push_back(false);
	genotype->push_back(false);
	genotype->push_back(true);
	genotype->push_back(false);
	genotype->push_back(true); // 00101 -> 15.625 for (0,100)

	genotype->push_back(false);
	genotype->push_back(false);
	genotype->push_back(true);
	genotype->push_back(true);
	genotype->push_back(true); // 00101 -> 21.855 for (0,100)

	// flip the genotype to gray
	genotype->binaryToGray();

	i = new Individual(genotype, 0);

	cout << problem->evaluate(i);
	CPPUNIT_ASSERT_DOUBLES_EQUAL(problem->evaluate(i), 1 / (1 + 37.5),
			0.0000001);
	delete genotype;

	cout << "\nworsts /----------" << endl;

	std::for_each(worsts->begin(), worsts->end(), [](Individual * i) {
		cout << i <<endl;
		CPPUNIT_ASSERT(i->isEvaluated() == true);
		CPPUNIT_ASSERT(i->birth() == 0);

	});
	pop->remove(worsts);

	CPPUNIT_ASSERT_EQUAL(8, (int) pop->size());

	CPPUNIT_ASSERT(worsts->at(0) != pop->at(0));
	CPPUNIT_ASSERT(worsts->at(0) != pop->at(1));
	CPPUNIT_ASSERT(worsts->at(0) != pop->at(2));

	CPPUNIT_ASSERT(worsts->at(1) != pop->at(0));
	CPPUNIT_ASSERT(worsts->at(1) != pop->at(1));
	CPPUNIT_ASSERT(worsts->at(1) != pop->at(2));

	pop->add(worsts);

	CPPUNIT_ASSERT_EQUAL(10, (int) pop->size());

	// be careful for equivalent objects the order of pointers can be different
	CPPUNIT_ASSERT_EQUAL(worsts->at(0)->fitness(), pop->at(0)->fitness());
	CPPUNIT_ASSERT_EQUAL(worsts->at(1)->fitness(), pop->at(1)->fitness());

	pop->remove(worsts);

	while (worsts->size() > 0) {
		Individual * i = worsts->back();
		worsts->pop_back();
		delete i;
	}

	delete worsts;

}

void Population_test::setUp(void) {
	config = new Config(string("utest/config"));
	problem = new ParseFunctionProblem(config);
	pop = new Population();
	int num_individuals = config->getInt(GA::POPULATION_SIZE_PARAM);
	int num_genes = config->getInt(GA::NUMBER_OF_GENES_PARAM);
	IContainer * individuals = new IContainer();

	for (int i = 0; i < num_individuals; i++) {
		Individual * ind = new Individual(num_genes, 0);
		ind->fitness(problem->evaluate(ind));
		ind->setEvaluated(true);

		individuals->push_back(ind);
	}

	pop->add(individuals);
	delete individuals;
}

void Population_test::tearDown(void) {
	delete pop;
	delete problem;
	delete config;
}

#endif /* UTEST_POPULATION_TEST_H_ */
