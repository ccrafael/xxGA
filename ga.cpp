/*
 * GeneticAlgorithm.cpp
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */
#include "ga.h"
#include "Config.h"

Config c("etc/config");
int GA::AllelesNumber = c.getInt(ALLELES_NUMBERS);
int GA::FitnessType = c.getInt(FITNESS_TYPE);
int GA::EncodingType = c.getInt(ENCODING_TYPE);
int GA::IndividualsNumber = c.getInt(INDIVIDUALS_NUMBER);
int GA::GenesNumber= c.getInt(GENES_NUMBER);
int GA::GenerationsNumber= c.getInt(GENERATIONS_NUMBERS);
int GA::TerminationConditionType= c.getInt(TERMINATION_CONDITION_TYPE);
int GA::PopulationInitializationType= c.getInt( POPULATION_INITIALIZATION_TYPE);
int GA::ParentSelectionType= c.getInt(PARENT_SELECTION_TYPE);
int GA::ExpectedOffspringNumberForFittestIndividual= c.getInt(EXPECTED_OFF_SPRING_NUMBER_FOR_FITNESS_INDIVIDUALS);
int GA::TournamentSize= c.getInt(TOURNAMENT_SIZE);
int GA::CrossoverType= c.getInt(CROSSOVER_TYPE);
double GA::CrossoverRate= c.getDouble(CROSSOVER_RATE);
int GA::CrossoverPointsNumber= c.getInt(CROSSOVER_POINTS_NUMBERS);
double GA::ExchangeProbability= c.getDouble(ECHANGE_PROBATILITY);
int GA::MutationType= c.getInt(MUTATION_TYPE);
double GA::MutationRate = c.getDouble(MUTATION_RATE);
int GA::SurvivorSelectionType= c.getInt(SURVIVOR_SELECTION_TYPE);
int GA::Elitism= c.getInt(ELITISM);
int GA::GenerationalGap= c.getInt(GENERATIONAL_GAP);

GA::GA(Phenotype * ph) {
	GA::generation = 0;
	GA::env = new Environment(ph);
}

GA::~GA() {
	delete GA::env;
}

void GA::run(void) {
	cout << " running for "<<GA::GenerationsNumber<<" generations. "<< endl;;
	env->population->init();
	env->evaluate( env->population->getIndividuals() );

	cout << "--- initial population -----------------------"<<endl;
	cout << env->population <<endl;

	while ( !isEnd() ) {

		IContainer parents = env->population->parentsSelection();

		for (unsigned int i = 0; i < parents.size(); i ++) {
			cout << "  -parent "<<i<<":"<<parents.at(i)<<endl;
		}

		IContainer offspring = env->recombine( parents );

		for (unsigned int i = 0; i < offspring.size(); i ++) {
			cout << "   hijo   "<<i<<":"<<offspring.at(i)<< " <-- before mutate" <<endl;
		}
		env->mutate( offspring );

		env->evaluate( offspring );

		for (unsigned int i = 0; i < offspring.size(); i ++) {
			cout << "  +hijo   "<<i<<":"<<offspring.at(i)<< endl;
		}

		env->population->generationReplacement( offspring );

		cout << "--- population "<< generation <<" -----------------------------"<<endl;
		cout << env->population <<endl;

		GA::generation ++;
	}

	cout << "--- population "<< generation <<" decoded -----------------------"<<endl;
	cout << *env <<endl;

	cout << " end running ..."<<endl;;
}

bool GA::isEnd() {
	return ( GA::generation > GA::GenerationsNumber );
}


