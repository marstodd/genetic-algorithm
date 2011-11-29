#include "Population.h"


//relates the fitness of a pocket to its array/vector index for easy sorting.



Population::Population(void)
{
}

Population::Population(int initPopulationSize, int initPocketSize, bool initCoinsAllowed[], double initElitismCutoffPercent, int initTargetValue = 0)
{
	this->populationSize = initPopulationSize;
	this->pocketSize = initPocketSize;
	this->targetValue = initTargetValue;
	this->elitismCutOffPercent = initElitismCutoffPercent;
	//copy the allowed coins into this->coinsAllowed before passing it to the pockets
	for(int index = 0; index < NUMBER_OF_COINS; index++)
	{
		this->coinsAllowed[index] = initCoinsAllowed[index];
	}

	//establish pockets array
	for(int index = 0; index < this->populationSize; index++)
	{
		population.push_back(new Pocket(this->coinsAllowed,this->pocketSize, this->targetValue));
	}

}

Population::~Population(void)
{
}

void Population::QuickSort(SortData toSort, int left, int right)
{
	
	int i = left, j = right;
	int tmp;
	int pivot = toSort.fitness[(left + right) / 2];
 
	/* partition */
	while (i <= j) {
		while (toSort.fitness[i] < pivot)
				i++;
		while (toSort.fitness[j] > pivot)
				j--;
		if (i <= j) {
				tmp = toSort.fitness[i];
				toSort.fitness[i] = toSort.fitness[j];
				toSort.fitness[j] = tmp;

				//keep the array index linked to the fitness
				tmp = toSort.arrayIndex[i];
				toSort.arrayIndex[i] = toSort.arrayIndex[j];
				toSort.arrayIndex[j] = tmp;

				i++;
				j--;
		}
	};
 
	/* recursion */
	if (left < j)
		this->QuickSort(toSort, left, j);
	if (i < right)
		this->QuickSort(toSort, i, right);

}

void Population::Sort()
{
	SortData toSort;
	toSort.arrayIndex = new int [this->populationSize];
	toSort.fitness = new int [this->populationSize];

	//fill the arrays with population data
	for(int index = 0; index < this->populationSize; index++)
	{
		toSort.arrayIndex[index] = index;
		toSort.fitness[index] = population[index]->GetFitness();
	}

	this->QuickSort(toSort,0,this->populationSize - 1);


	//make a temp vector and fill it with the sorted list, then swap it with the current population
	vector<Pocket*> temp;
	for(int index = 0; index < this->populationSize; index++)
	{
		temp.push_back(this->population[toSort.arrayIndex[index]]);
	}
	this->population.swap(temp);
	

	//display test
	/*for(int i = 0; i < this->populationSize; i++)
	{
		cout << this->population[i]->GetFitness() << endl;
	}*/
}

void Population::RandomizePopulation()
{
	for(int index = 0; index < this->populationSize; index++)
	{
		this->population[index]->Randomize();
	}
}

vector<Pocket*> Population::PerformCrossover(int parentIndex1, int parentIndex2)
{
	vector<Pocket*> children;
	vector<int> parentCoins1 = this->population[parentIndex1]->GetCoins();
	vector<int> parentCoins2 = this->population[parentIndex2]->GetCoins();
	vector<int> childCoins1;
	vector<int> childCoins2;

	//cout << "Crossover starting" << endl;

	//make the vectors large enough to hold the coins
	childCoins1.resize(this->pocketSize);
	childCoins2.resize(this->pocketSize);

	int crossOverSplit = this->RNG.next(1,this->pocketSize - 1); //Holds the value/index of where the two parents pockets will be split to form the new children
	//the two arguments (1, pocketSize - 1) are used to ensure that at least one coin is crossed over. If 0 were chosen, no crossover would happen and same for the upper bound

	//fills the childrens pockets with coins from parents until crossover point, then switch parents and fill the rest of the pocket

	for(int index = 0; index < crossOverSplit; index++)
	{
		childCoins1[index] = parentCoins1[index];
		childCoins2[index] = parentCoins2[index];
	}

	for(int index = crossOverSplit; index < this->pocketSize; index++)
	{
		childCoins1[index] = parentCoins2[index];
		childCoins2[index] = parentCoins1[index];
	}

	Pocket* child1 = new Pocket(this->coinsAllowed,this->pocketSize, this->targetValue);
	Pocket* child2 = new Pocket(this->coinsAllowed,this->pocketSize, this->targetValue);
	child1->SetCoins(childCoins1);
	child2->SetCoins(childCoins2);
	children.push_back(child1);
	children.push_back(child2);
	return children;

}

vector<Pocket*> Population::PerformElitism()
{
	vector<Pocket*> elites;
	int eliteCount = this->populationSize * this->elitismCutOffPercent;
	if(eliteCount == 0) eliteCount = 1; //always keep at least one
	//cout << "Number to keep: " << eliteCount << endl;
	this->Sort();
	for(int index = 0; index < eliteCount; index++)
	{
		elites.push_back(this->population[index]);
	}

	return elites;	
}

void Population::DisplayData(int populationNumber)
{
	double averageFitness = 0;
	int totalFitness = 0;
	this->Sort();
	vector<int> coinComposition = population[0]->GetCoins();
	int coinCount[NUMBER_OF_COINS] = {0};

	for(int index = 0; index < this->pocketSize; index++)
	{
		if(coinComposition[index] == Coins::PENNY) coinCount[0]++;
		else if(coinComposition[index] == Coins::NICKEL) coinCount[1]++;
		else if(coinComposition[index] == Coins::DIME) coinCount[2]++;
		else if(coinComposition[index] == Coins::QUARTER) coinCount[3]++;
		else if(coinComposition[index] == Coins::HALF_DOLLAR) coinCount[4]++;
		else if(coinComposition[index] == Coins::DOLLAR) coinCount[5]++;
	}

	//takes the average fitness of the population, lower is better
	for(int index = 0; index < this->population.size(); index++)
	{
		totalFitness += population[index]->GetFitness();
	}
	averageFitness = (totalFitness / this->population.size());

	cout << "Generation #" << populationNumber << " population data." << endl
		 << "Average Fitness: " << averageFitness << endl
		 << "Most Fit: " << population[0]->GetFitness() << endl
		 << "Coin Composition of most fit: " << endl
		 << "\tPenny:       " << coinCount[0] << endl
		 << "\tNickel:      " << coinCount[1] << endl
		 << "\tDime:        " << coinCount[2] << endl
		 << "\tQuarter:     " << coinCount[3] << endl
		 << "\tHalf-Dollar: " << coinCount[4] << endl
		 << "\tDollar:      " << coinCount[5] << endl;
}

vector<Pocket*> Population::GetPockets()
{
	return this->population;
}

void Population::SetPockets(vector<Pocket*> newPopulation)
{
	this->population = newPopulation;
}

void Population::Display()
{
	for(int i = 0; i < this->populationSize; i++)
	{
		population[i]->Display();
	}
}