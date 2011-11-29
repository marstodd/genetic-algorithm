#include "Random.h"
#include <cstdlib>
#include <ctime>

Random::Random() {
	// Seed the random number generator
	srand(time(0));
}

Random::~Random() {
}

/*
 * Return the next random integer
 */
int Random::next()
{
	return rand();
}

/*
 * Return the next random integer greater than or
 * equal to 0 and smaller than the upper limit
 */
int Random::next(int upperLimit)
{
	return rand() % upperLimit;
}

/*
 * Return the next random integer greater than or equal to the
 * lower limit and less than the upper limit
 */
int Random::next(int lowerLimit, int upperLimit)
{
	return (rand() % (upperLimit - lowerLimit)) + lowerLimit;
}