/*
 * OperatorFactory.cpp
 *
 *  Created on: 24 Nov 2017
 *      Author: fali
 */

#include "OperatorFactory.h"
log4cxx::LoggerPtr OperatorFactory::logger(
		log4cxx::Logger::getLogger("operatorFactory"));

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

	case 1: {
		LOG4CXX_TRACE(logger, "Crossover: One point crossover");
		return crossover::onepoint;
	}
	case 4: {
		LOG4CXX_TRACE(logger, "Crossover: N point crossover");
		return crossover::npoint;
	}
	default: {
		LOG4CXX_TRACE(logger, "Crossover: One point crossover");
		return crossover::onepoint;
	}

	}

}

/*
 * Create a new mutation operator.
 * @return A new mutation operator.
 */
std::function<void(IContainer*)> OperatorFactory::createMutationOperator() {

	switch (config->getInt("MutationType")) {
	case 1:
		return mutation::basic;
	case 2:
		return mutation::rate;
	default:
		return mutation::basic;
	}
}

/*
 * Create a new selection operator.
 * @return A new selection operator.
 */
std::function<IContainer* (Population*, unsigned int)> OperatorFactory::createParentSelectionOperator() {
	switch (config->getInt("ParentSelectionType")) {
	case 1: {
		LOG4CXX_TRACE(logger, "Selection: Fitness proportional selection. ");
		return selection::fitnessproportional;
	}
	case 2: {
		LOG4CXX_TRACE(logger, "Selection: Stochastic universal sampling. ");

		return selection::stochasticuniversalsampling;
	}
	case 3: {

		LOG4CXX_TRACE(logger, "Selection: Ranking selection. ");
		return selection::ranking;
	}
	case 4: {
		LOG4CXX_TRACE(logger, "Selection: Tournament selection. ");
		return selection::tournament;
	}
	default: {

		LOG4CXX_TRACE(logger, "Selection: Basic (random) selection. ");
		return selection::basic;
	}
	}

}

/*
 * Create a new replacement selection operator.
 * @return A new replacement selection operator.
 */
std::function<IContainer* (Population*, IContainer*)> OperatorFactory::createReplacementSelectionOperator() {
	switch (config->getInt("SurvivorSelectionType")) {
	case 1: {
		LOG4CXX_TRACE(logger, "Replacement: generational  ");

		return replacement::generational;
	}
	case 2: {
		LOG4CXX_TRACE(logger, "Replacement: steady state ");
		return replacement::steadystate;
	}
	case 3: {
		return replacement::ifbetter;
	}
	default: {
		LOG4CXX_TRACE(logger, "Replacement: empty ");
		return replacement::empty;
	}
	}

}

/*
 * Create a new Emmigration selection operator.
 * @return A new migration selection operator.
 */
std::function<IContainer* (Population*, unsigned int)> OperatorFactory::createEmigrationSelectionOperator() {
	switch (config->getInt("EmigrationType")) {
	case 1: {

		LOG4CXX_TRACE(logger, "Emigration: basicmigration ");
		return selection::basic_migration;
	}
	case 2: {

		LOG4CXX_TRACE(logger, "Emigration: worst. ");
		return selection::worsts_migration;
	}
	case 3: {

		LOG4CXX_TRACE(logger, "Emigration: bests. ");
		return selection::bests_migration;
	}
	default: {

		LOG4CXX_TRACE(logger, "Emigration: basic. ");
		return selection::basic_migration;
	}
	}

}

/*
 * Create a new Immigration selection operator.
 * @return A new migration selection operator.
 */
std::function<IContainer* (Population*, unsigned int)> OperatorFactory::createImmigrationSelectionOperator() {
	switch (config->getInt("ImmigrationType")) {
	case 1: {

		LOG4CXX_TRACE(logger, "Immigration: basicmigration ");
		return selection::basic_migration;
	}
	case 2: {

		LOG4CXX_TRACE(logger, "Immigration: worst. ");
		return selection::worsts_migration;
	}
	case 3: {

		LOG4CXX_TRACE(logger, "Immigration: bests. ");
		return selection::bests_migration;
	}
	default: {

		LOG4CXX_TRACE(logger, "Immigration: basic. ");
		return selection::basic_migration;
	}
	}
}

std::function<void(Problem*, IContainer*)> OperatorFactory::createEvaluationOperator() {
	switch (config->getInt("EvaluationType")) {
	case 1:
		return evaluation::basicevaluation;
	case 2:
		return evaluation::openclevaluation;

	default:
		return evaluation::basicevaluation;
	}
}

