#pragma once
#include<vector>
#include "Random.h"
#include<math.h>
#include<iostream>
using std::cout;
using std::endl;
using std::vector;
#define NUMBER_OF_COINS 6

enum Coins
{
	PENNY = 1,
	NICKEL = 5,
	DIME = 10,
	QUARTER = 25,
	HALF_DOLLAR = 50,
	DOLLAR = 100
};

class Pocket
{
private:
	bool coinsAllowed[NUMBER_OF_COINS]; //The user selects which coins are used, this array keeps track of which coins can be used and which ones can't
	vector<int> coins;
	int numberOfCoinsInPocket;
	Random RNG;

	int targetValue; //the desired amount of money in pocket (defined by user) the value is multiplied by 100 to work with the enum
	//i.e. $5.01 = 501
	int currentValue;
	int fitness; // |targetValue - currentValue|  -- lower number is better

	int GetCoin(); //Returns a random coin for use with Randomize() and Mutate(), not to be confused with GetCoins
public:

	void UpdateStats(); //updates 

	void Randomize(); //sets all slots in pocket to a random coin
	void Mutate(); //changes the value of ONE slot to a random coin(changes one coin, can remain the same)

	//test function
	void Display();
	int GetFitness();
	vector<int> GetCoins(); //returns array of coins, used most noteably in Population::PerformCrossover()
	void SetCoins(vector<int>); //sets the coins in pocket from an array rather than randomizing, used mostly in Population::PerformCrossover() when generating new children.

	Pocket(bool initCoinsAllowed[], int initNumberOfCoinsInPocket, int targetValue);
	Pocket(void);
	~Pocket(void);
};

