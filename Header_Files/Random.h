/*
 * Random.h
 *
 *  Created on: Feb 20, 2010
 *      Author: hartley
 */

#pragma once
#ifndef RANDOM_H_
#define RANDOM_H_

class Random {
public:
	Random();
	virtual ~Random();
	int next();
	int next(int);
	int next(int, int);
};

#endif /* RANDOM_H_ */
