/*
 * GeneticAlgorithm.h
 *
 *  Created on: 26-nov-2008
 *      Author: rafael
 */
#ifndef GENETICALGORITHM_H_
#define GENETICALGORITHM_H_

#include "sys.h"
#include "Environment.h"
using namespace std;

/**
 * como hacemos para llamar a las distintas funcionalidades.
 * Por ejemplo para llamar a un metodo de seleccion distinto de otro,
 */
class GA {
	Environment * env;
	int generation;
public:

	GA(Phenotype * ph);
	virtual ~GA();

	void run();
	static void init();

	/*
	 * Type of fitness/problem to be applied/solved by GA:
	 * 1: Maximization of a function defined on real numbers
	 * 2: Maximization of the One-Max function
	 * ...
	 */
	static int FitnessType;

	/*
	 * Type of encondig
	 * 1: Binary encoding, Genotype(0 1 0 1) -> Phenotype(5)
	 * 2: Gray encoding,   Genotype(0 1 0 1) -> Phenotype(6) (See 'GAs + Data Structures = Evolution Programs', page 99)
	 * ...
	 */
	static int EncodingType;

	/*
	 * Number of individuals in the population
	 */
	static int IndividualsNumber;

	/*
	 * Number of genes for each individual
	 */
	static int GenesNumber;

	/*
	 * Number of alleles for each gene
	 */
	static int AllelesNumber;

	/*
	 * Number of generations
	 */
	static int GenerationsNumber;

	/*
	 * Termination condition for the evolutionary algorithm:
	 * 1: GenerationsNumber is interpreted as the number of generations for the algorithm
	 * 2: GenerationsNumber is interpreted as the maximum number of generations allowed without change in the fitness of the best individual
	 * ...
	 */
	static int TerminationConditionType;

	/*
	 * Type of population initialization
	 * 1: Random
	 * ...
	 */
	static int PopulationInitializationType;

	/*
	 * Type of parent selection (method of constructing the mating pool):
	 * 1: Fitness-proportional selection
	 * 2: Stochastic universal sampling
	 * 3: Ranking selection (linear)
	 * 4: Tournament selection
	 * ...
	 */
	static int ParentSelectionType;

	/*
	 * Expected number of offspring alloted to the fittest individual (just in case of linear ranking selection). Range: [1,2]
	 */
	static int ExpectedOffspringNumberForFittestIndividual;

	/*
	 * Tournament size (just in case of tournament selection). Range: [2,IndividualsNumber].
	 */
	static int TournamentSize;

	/*
	 * Type of crossover:
	 * 1: One-point crossover
	 * 2: Two-point crossover
	 * 3: N-point crossover
	 * 4: Parameterized uniform crossover
	 * ...
	 */
	static int CrossoverType;

	/*
	 * Crossover rate
	 */
	static double CrossoverRate;

	/*
	 * Number of crossover points (just in case of N-point crossover). Range: [1,GenesNumber-1]
	 */
	static int CrossoverPointsNumber;

	/**
	 * Exchange probability (just in case of Parameterized Uniform Crossover)
	 */
	static double ExchangeProbability;

	/*
	 * Type of mutation:
	 * 1: Point probabilistic mutation
	 * ...
	 */
	static int MutationType;

	/*
	 * Mutation rate
	 */
	static double MutationRate;

	/*
	 * Type of survivor selection:
	 * 1: Generational model
	 * 2: Steady state model
	 *  ...
	 */
	static int SurvivorSelectionType;

	/*
	 * Elistism (just in case of generational model for survivor selection)
	 * 1: ON
	 * 2: OFF
	 */
	static int Elitism;

	//Generational gap (just in case of steady state model for survivor
	//  selection). Range: [1, IndividualsNumber].
	static int GenerationalGap ;
private:
	bool isEnd();
};

#endif /* GENETICALGORITHM_H_ */
