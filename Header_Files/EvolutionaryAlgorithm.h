#pragma once
#include "Population.h"

class EvolutionaryAlgorithm
{
private:
	Population* population; //current and seed population
	vector<Pocket*> temp; //population that holds newly generated children with current population members, tournament selection is used to copy them to survivors
	vector<Pocket*> survivors; //records the pockets chosen for the next generation
	Random RNG;

	int generations;
	int childrenPerGeneration;
	int tournamentSize;
	int mutationRate;
	int currentGeneration;

	int numberOfPocketsInSurvivors;


	//variables for lower classes
	bool coinsAllowed[NUMBER_OF_COINS];
	int populationSize;
	int pocketSize;
	double eliteCutOffPercent;
	int targetValue;

	void PerformTournament();
	void PerformMutation();
	void MakeBabies();
	//void PerformCrossover();
	void PerformElitism(); //copies the elite straight to survivors without mutations
	void RandomDeaths(); //kills of members of the population at random
	void CopyCurrentPopulationToTemp();
	void DisplayData();
	void GetInput(bool coinsAllowed[]); //gets all necessary input from the user and stores it in the instance variables
public:
	void Start();
	EvolutionaryAlgorithm(void);
	~EvolutionaryAlgorithm(void);
};

