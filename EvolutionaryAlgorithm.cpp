#include "EvolutionaryAlgorithm.h"
#include <iostream>
using std::cin;
using std::cout;
using std::endl;


EvolutionaryAlgorithm::EvolutionaryAlgorithm(void)
{
	this->GetInput(coinsAllowed);
	this->currentGeneration = 0;
	temp.resize(this->populationSize + this->childrenPerGeneration);
	survivors.resize(this->populationSize);

	//Create the population of pockets
	population = new Population(this->populationSize,this->pocketSize,this->coinsAllowed,this->eliteCutOffPercent,this->targetValue);
}

void EvolutionaryAlgorithm::Start()
{
	this->currentGeneration = 0;
	cout << "Randomizing initial population. . ." << endl;
	population->RandomizePopulation();
	cout << "Randomization complete, displaying initial data:" << endl;
	this->DisplayData();
	this->currentGeneration++;
	//now for the meat of the system
	for(this->currentGeneration = 0; this->currentGeneration < this->generations; this->currentGeneration++)
	{	

		//clear and resize vectors for next generation
		this->temp.clear();
		this->temp.resize(this->populationSize + this->childrenPerGeneration);
		//empty the survivors to serve as a clean slate for the next generation
		this->survivors.clear();
		this->survivors.resize(this->populationSize);

		this->CopyCurrentPopulationToTemp();
		this->RandomDeaths();
		this->PerformElitism();
		this->MakeBabies();
		this->PerformMutation();
		this->PerformTournament();
		this->population->SetPockets(this->survivors);
		this->DisplayData();
	//	system("PAUSE");
	}
}

void EvolutionaryAlgorithm::PerformTournament()
{
	int* tournamentSeeds = new int [this->tournamentSize];
	for(int index = this->numberOfPocketsInSurvivors; index < this->populationSize; index++)
	{
		//get tourney seeds
		for(int tourneyIndex = 0; tourneyIndex < this->tournamentSize; tourneyIndex++)
		{
			tournamentSeeds[tourneyIndex] = RNG.next(this->childrenPerGeneration + this->populationSize);
		}

		int winnderIndex = tournamentSeeds[0];
		//determine winnder
		for(int i = 1; i < this->tournamentSize; i++)
		{
			if(temp[winnderIndex]->GetFitness() > tournamentSeeds[i]) winnderIndex = tournamentSeeds[i];
		}
		this->survivors[index] = temp[winnderIndex];

	}
	delete [] tournamentSeeds;
}

void EvolutionaryAlgorithm::MakeBabies()
{
	int randomParent1;
	int randomParent2;
	vector<Pocket*> children;
	//children.resize(this->childrenPerGeneration);
	for(int index = 0; index < this->childrenPerGeneration; index+=2)
	{
		randomParent1 = RNG.next(this->populationSize);
		randomParent2 = RNG.next(this->populationSize);
		children = this->population->PerformCrossover(randomParent1, randomParent2);
		temp[this->populationSize + index] = children[0];
		temp[this->populationSize + index + 1] = children[1];
		children.clear();
	}
}

void EvolutionaryAlgorithm::PerformMutation()
{
	for(int index = 0; index < this->temp.size(); index++)
	{
		if(RNG.next(100) < this->mutationRate)
		{
			this->temp[index]->Mutate();
		}
	}
}

void EvolutionaryAlgorithm::PerformElitism()
{
	this->numberOfPocketsInSurvivors = 0;
	vector<Pocket*> elites = this->population->PerformElitism();
	for(int index = 0; index < elites.size(); index++)
	{
		this->survivors[index] = elites[index];
		this->numberOfPocketsInSurvivors++;
	}
	
}

void EvolutionaryAlgorithm::CopyCurrentPopulationToTemp()
{
	vector<Pocket*> currentPopulation = this->population->GetPockets();
	currentPopulation.resize(this->populationSize + this->childrenPerGeneration);
	this->temp = currentPopulation;
}

void EvolutionaryAlgorithm::DisplayData()
{
	this->population->DisplayData(this->currentGeneration);
}

void EvolutionaryAlgorithm::GetInput(bool coinsAllowed[])
{
//	bool coinsAllowed[NUMBER_OF_COINS];
	int populationSize;
	int pocketSize;
	int numberOfGenerations;
	int childrenPerGeneration;
	int tournamentSize;
	double eliteCutOffPercent;
	double targetValue; 
	double mutationRate;

	cout << "Select the parameters by entering integers greater than 0 unless specified otherwise." << endl;
	//get population size
	do
	{
		cout << "Population Size: ";
		cin.clear();
		cin.sync();
		cin >> populationSize;
	}while(cin.fail() || populationSize <= 0);
	this->populationSize = populationSize;

	//# of generations
	do
	{
		cout << "Number of generations: ";
		cin.clear();
		cin.sync();
		cin >> numberOfGenerations;
	}while(cin.fail() || numberOfGenerations <= 0);
	this->generations = numberOfGenerations;

	//children per generation
	do
	{
		cout << "Children per generation (must be an even number): ";
		cin.clear();
		cin.sync();
		cin >> childrenPerGeneration;
	}while(cin.fail() || childrenPerGeneration <= 0 || childrenPerGeneration % 2 != 0);
	this->childrenPerGeneration = childrenPerGeneration;

	//mutation rate
	do
	{
		cout << "Mutation rate (must be between 0 and 1): ";
		cin.clear();
		cin.sync();
		cin >> mutationRate;
	}while(cin.fail() || mutationRate <= 0 || mutationRate >= 1);
	this->mutationRate = mutationRate * 100;

	//tournament size
	do
	{
		cout << "Tournament Size (must be greater than one): ";
		cin.clear();
		cin.sync();
		cin >> tournamentSize;
	}while(cin.fail() || tournamentSize <= 1);
	this->tournamentSize = tournamentSize;

	//get pocket size (number of coins in pocket)
	do
	{
		cout << "Pocket Size: ";
		cin.clear();
		cin.sync();
		cin >> pocketSize;
	}while(cin.fail() || pocketSize <= 0);
	this->pocketSize = pocketSize;

	//get eliteCutOffPercent
	do
	{
		cin.clear();
		cin.sync();
		cout << "Enter an elitism cutoff percent, this must be a number between 0 and 1." << endl;
		cout << "Elitism cutoff: ";
		cin >> eliteCutOffPercent;
	}while(cin.fail() || eliteCutOffPercent <= 0 || eliteCutOffPercent >= 1);
	this->eliteCutOffPercent = eliteCutOffPercent;

	//target value gets converted to an int later 100x the amount, $5.33 will become 533.
	do
	{
		cout << "Enter a target value to determine the preferred amount of money in the pocket (no dollar sign) \n"
			 << "I.E. 5.33 for five dollars and thirty-three cents." << endl;
		cout << "Enter a target value: ";
		cin.clear();
		cin.sync();
		cin >> targetValue;
	}while(cin.fail() || targetValue <= 0);
	this->targetValue = (targetValue * 100);
	if(this->targetValue < this->pocketSize)
	{
		cout << "The target value is too small to be reached by the defined pocket size, but the algorithm will try to get close." << endl;
	}
	else if(this->targetValue > this->pocketSize * 100)
	{
		cout << "The target value is too large to be reached by the defined pocket size, but the algorithm will try to get close." << endl;
	}

	//now for the coin input, uses a temporary int to facilitate the process, user will enter 0 to disallow a coin, and any other number to allow it
	cout << "For this section, you will choose to allow or disallow certain coins. To disallow, enter 0. To allow, enter any other integer." << endl;
	int temp[NUMBER_OF_COINS];
	//penny
	do
	{
		cout << "Penny: ";
		cin.clear();
		cin.sync();
		cin >> temp[0];
	}while(cin.fail());
	//nickel
	do
	{
		cout << "Nickel: ";
		cin.clear();
		cin.sync();
		cin >> temp[1];
	}while(cin.fail());
	//dime
	do
	{
		cout << "Dime: ";
		cin.clear();
		cin.sync();
		cin >> temp[2];
	}while(cin.fail());
	do
	{
		cout << "Quarter: ";
		cin.clear();
		cin.sync();
		cin >> temp[3];
	}while(cin.fail());
	do
	{
		cout << "Half-Dollar: ";
		cin.clear();
		cin.sync();
		cin >> temp[4];
	}while(cin.fail());
	do
	{
		cout << "Dollar: ";
		cin.clear();
		cin.sync();
		cin >> temp[5];
	}while(cin.fail());

	for(int index = 0; index < NUMBER_OF_COINS; index++)
	{
		if(temp[index] == 0) this->coinsAllowed[index] = false;
		else this->coinsAllowed[index] = true;
	}

}

void EvolutionaryAlgorithm::RandomDeaths()
{
	//TODO: Have user add a random death rate
}

EvolutionaryAlgorithm::~EvolutionaryAlgorithm(void)
{
}
