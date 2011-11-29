#pragma once
#include "Pocket.h"
#include <vector>
using std::vector;

struct SortData
{
	int* arrayIndex;
	int* fitness;
};



//#define POCKET_SIZE 100

class Population
{
private:
	int populationSize;
	int pocketSize;
	int targetValue;
	bool coinsAllowed[NUMBER_OF_COINS]; //although contained in each pocket instance, this is necessary for CrossOver to generate the new children
	double elitismCutOffPercent; //0-100, denotes the amount of pockets get transferred to the next generation automatically
	//Pocket** pockets; //array of pockets in population
	vector<Pocket*> population;
	Random RNG;

	void QuickSort(SortData toSort, int left, int right); //source: http://www.algolist.net/Algorithms/Sorting/Quicksort

public:
	void RandomizePopulation();
	void Sort(); //does the necessary setup for QuickSort()
	void DisplayData(int populationNumber);
	vector<Pocket*> PerformCrossover(int parentIndex1, int parentIndex2);
	vector<Pocket*> PerformElitism(); //cutOffPercent will be 0-100, not 0-1

	vector<Pocket*> GetPockets();
	void SetPockets(vector<Pocket*>);

	void Display(); //test function

	Population(void);
	Population(int initPopulationSize, int initPocketSize, bool coinsAllowed[NUMBER_OF_COINS], double initElitismCutoffPercent, int initTargetValue);
	~Population(void);
};

