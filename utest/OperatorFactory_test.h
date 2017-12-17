/*
 * OperatorFactory_test.h
 *
 *  Created on: 9 Dec 2017
 *      Author: fali
 */

#ifndef UTEST_OPERATORFACTORY_TEST_H_
#define UTEST_OPERATORFACTORY_TEST_H_

#include <sstream>
#include <iostream>
#include "utests.h"
#include "../Individual.h"
#include "../GenotypeBit.h"
#include "../OperatorFactory.h"
#include "../Problem.h"
#include "../Config.h"
#include "../IContainer.h"
#include "../Context.h"
#include "../ga.h"

/*
 * A unit test for Individual and genotypebit.
 */

class OperatorFactory_test: public CppUnit::TestFixture {
CPPUNIT_TEST_SUITE(OperatorFactory_test);
	CPPUNIT_TEST(test);

	CPPUNIT_TEST_SUITE_END()
	;

public:
	void setUp(void);
	void tearDown(void);

protected:
	void test(void);

private:
	OperatorFactory * factory;
	Config * config;
	Problem * problem;
	Population * population;
	Context * context;

};

void OperatorFactory_test::test(void) {

	cout << " select the parents " << endl;
	std::function<IContainer* (Population*)> parentselection =
			factory->createParentSelectionOperator();

	IContainer * parents = parentselection(population);

	CPPUNIT_ASSERT(parents != nullptr);
	CPPUNIT_ASSERT_EQUAL(2, (int )parents->size());

	cout << " perform crossover " << endl;

	std::function<IContainer* (IContainer*, int)> crossoverop =
			factory->createCrossoverOperator();
	IContainer * offspring = crossoverop(parents, 1);

	CPPUNIT_ASSERT(offspring != nullptr);
	CPPUNIT_ASSERT_EQUAL(2, (int )offspring->size());

	for_each(offspring->begin(), offspring->end(),
			[this,parents](Individual *i) {
				CPPUNIT_ASSERT_EQUAL(1, i->birth());
				CPPUNIT_ASSERT_EQUAL(10, i->get_genotype()->size());
				CPPUNIT_ASSERT_EQUAL(false, i->isEvaluated());
				CPPUNIT_ASSERT(i != parents->at(0));
				CPPUNIT_ASSERT(i != parents->at(1));
			});

	std::function<void(IContainer*)> mutationop =
			factory->createMutationOperator();
	mutationop(offspring);
	CPPUNIT_ASSERT(offspring != nullptr);
	CPPUNIT_ASSERT_EQUAL(2, (int )offspring->size());

	for_each(offspring->begin(), offspring->end(),
			[this,parents](Individual *i) {
				CPPUNIT_ASSERT_EQUAL(1, i->birth());
				CPPUNIT_ASSERT_EQUAL(10, i->get_genotype()->size());
				CPPUNIT_ASSERT_EQUAL(false, i->isEvaluated());
				CPPUNIT_ASSERT(i != parents->at(0));
				CPPUNIT_ASSERT(i != parents->at(1));
			});

	std::function<void(Problem*, IContainer*)> eval =
			factory->createEvaluationOperator();
	eval(problem, offspring);

	for_each(offspring->begin(), offspring->end(),
			[this,parents](Individual *i) {
				CPPUNIT_ASSERT_EQUAL(1, i->birth());
				CPPUNIT_ASSERT_EQUAL(10, i->get_genotype()->size());
				CPPUNIT_ASSERT_EQUAL(true, i->isEvaluated());
				CPPUNIT_ASSERT(i != parents->at(0));
				CPPUNIT_ASSERT(i != parents->at(1));
			});

	std::function<IContainer* (Population*, IContainer*)> re =
			factory->createReplacementSelectionOperator();

	IContainer * discard = re(population, offspring);
	CPPUNIT_ASSERT_EQUAL(2, (int )discard->size());
	CPPUNIT_ASSERT_EQUAL(12, population->size());
	for_each(discard->begin(), discard->end(), [this,offspring](Individual *i) {
		CPPUNIT_ASSERT_EQUAL(0, i->birth());
		CPPUNIT_ASSERT_EQUAL(10, i->get_genotype()->size());
		CPPUNIT_ASSERT_EQUAL(true, i->isEvaluated());
		CPPUNIT_ASSERT(i != offspring->at(0));
		CPPUNIT_ASSERT(i != offspring->at(1));
	});

	population->remove(discard);

	for_each(discard->begin(), discard->end(),
			[](Individual *i) {cout << i <<endl;});

	while (discard->size() > 0) {
		Individual * i = discard->back();
		discard->pop_back();
		delete i;
	}

	delete discard;
	delete offspring;
	delete parents;
	CPPUNIT_ASSERT_EQUAL(10, population->size());
}

void OperatorFactory_test::setUp(void) {
	context = Context::instance();

	config = new Config(string("utest/config"));
	context->config = config;
	context->num_parents = config->getInt("NumParents");

	problem = new ParseFunctionProblem(config);
	factory = new OperatorFactory(config);
	population = new Population();


	IContainer * individuals = new IContainer();

	int num_individuals = config->getInt(GA::POPULATION_SIZE_PARAM);
	int num_genes = config->getInt(GA::NUMBER_OF_GENES_PARAM);

	for (int i = 0; i < num_individuals; i++) {
		Individual * ind = new Individual(num_genes, 0);
		ind->fitness(problem->evaluate(ind));
		ind->setEvaluated(true);

		individuals->push_back(ind);
	}

	population->add(individuals);

	delete individuals;
}

void OperatorFactory_test::tearDown(void) {
	delete factory;
	delete problem;
	delete config;

	delete population;
}

#endif /* UTEST_OPERATORFACTORY_TEST_H_ */
