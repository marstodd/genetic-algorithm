#include "Pocket.h"

void Pocket::UpdateStats()
{
	this->currentValue = 0;
	for(int counter = 0; counter < this->numberOfCoinsInPocket; counter++)
	{
		this->currentValue += coins[counter];
	}
	this->fitness = abs(this->targetValue - this->currentValue);
}

int Pocket::GetCoin()
{
	//loop until a valid coin is found (the user can decide to exclude certain coins)
	while(1)
	{
		int coin = RNG.next(NUMBER_OF_COINS);
		if(this->coinsAllowed[coin])
		{
			//the value of the coin is saved in the array, switch statement puts in the correct value for the chosen coin
			switch (coin)
			{
			case 0:
				return Coins::PENNY;
			case 1:
				return Coins::NICKEL;
			case 2:
				return Coins::DIME;
			case 3:
				return Coins::QUARTER;
			case 4:
				return Coins::HALF_DOLLAR;
			case 5:
				return Coins::DOLLAR;
			default:
				cout << "ERROR! Invalid coin detected." << endl;
				break;
			}
		}
	}
}

void Pocket::Randomize()
{
	//put a random coin in each "slot" of the pocket
	for(int index = 0; index < this->numberOfCoinsInPocket; index++)
	{
		this->coins[index] = this->GetCoin();
	}
	this->UpdateStats();
}

void Pocket::Mutate()
{
	int index = RNG.next(this->numberOfCoinsInPocket);
	coins[index] = this->GetCoin();
	this->UpdateStats();
}

Pocket::Pocket(void)
{
	
}

Pocket::Pocket(bool initCoinsAllowed[], int initNumberOfCoinsInPocket, int initTargetValue)
{
	//copy over bools
	for(int index = 0; index < NUMBER_OF_COINS; index++)
	{
		this->coinsAllowed[index] = initCoinsAllowed[index];
	}

	this->numberOfCoinsInPocket = initNumberOfCoinsInPocket;
//	this->coins = new int [numberOfCoinsInPocket];
	this->coins.resize(this->numberOfCoinsInPocket);
	this->targetValue = initTargetValue;
}

void Pocket::Display()
{
	cout << "Fitness: " << this->fitness << endl;
	cout << "Target Value: " << this->targetValue << endl;
	cout << "Current Value: " << this->currentValue << endl;
	cout << "Coins in pocket: ";
	for(int i = 0; i < this->numberOfCoinsInPocket; i++)
	{
		cout << coins[i] << ", ";
	}
	cout << "end of pocket!" << endl;
}

vector<int> Pocket::GetCoins()
{
	return this->coins;
}

void Pocket::SetCoins(vector<int> newCoins)
{
	this->coins = newCoins;
	this->UpdateStats();
}

int Pocket::GetFitness()
{
	return this->fitness;
}


Pocket::~Pocket(void)
{
//	delete [] coins;
}
