/*
 * OperatorFactory.cpp
 *
 *  Created on: 24 Nov 2017
 *      Author: fali
 */

#include "OperatorFactory.h"

OperatorFactory::OperatorFactory(Config * config) {
	this->config = config;

}

OperatorFactory::~OperatorFactory() {
	// nothing to clean
}

/*
 * Create a new crossover operator.
 * @return A new cross overoperator.
 */
std::function<IContainer* (IContainer*, int)> OperatorFactory::createCrossoverOperator() {
	switch (config->getInt("CrossoverType")) {

	case 1: return crossover::onepoint;
	case 4: return crossover::npoint;
	default: return crossover::onepoint;

	}

}

/*
 * Create a new mutation operator.
 * @return A new mutation operator.
 */
std::function<void(IContainer*)> OperatorFactory::createMutationOperator() {
	return mutation::basic;
}

/*
 * Create a new selection operator.
 * @return A new selection operator.
 */
std::function<IContainer* (Population*)> OperatorFactory::createParentSelectionOperator() {
	switch (config->getInt("ParentSelectionType")) {
	case 1: return selection::fitnessproportional;
	case 2: return selection::stochasticuniversalsampling;
	case 3: return selection::ranking;
	case 4: return selection::tournament;
	default: return selection::basic;
	}

}

/*
 * Create a new replacement selection operator.
 * @return A new replacement selection operator.
 */
std::function<IContainer* (Population*, IContainer*)> OperatorFactory::createReplacementSelectionOperator() {
	switch (config->getInt("SurvivorSelectionType")) {
	case 1: return replacement::generational;
	case 2: return replacement::steadystate;

	default: return replacement::empty;
	}

}

/*
 * Create a new Emmigration selection operator.
 * @return A new migration selection operator.
 */
std::function<IContainer* (Population*)> OperatorFactory::createEmigrationSelectionOperator() {
	switch (config->getInt("MigrationType")) {
	case 1: return selection::basicmigration;
	case 2: return selection::worsts;
	default:	return selection::basicmigration;
	}

}

/*
 * Create a new Immigration selection operator.
 * @return A new migration selection operator.
 */
std::function<IContainer* (Population*)> OperatorFactory::createImmigrationSelectionOperator() {
	return selection::basicmigration;
}


std::function<void (Problem*, IContainer*)> OperatorFactory::createEvaluationOperator() {
	return evaluation::basicevaluation;
}


