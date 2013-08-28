/*
 * Owner:Nuray Dindar, Ozgur Oguz
 * Summary:Simulator of the game.
 * Date: March 04, 2011 
*/
#pragma once
#ifndef _RLSIMULATION_HPP
#define _RLSIMULATION_HPP

#include "RLPlayer.hpp"

class RLSimulator
{

public:
	RLSimulator(void);
	~RLSimulator(void)	{
		delete pl1;
		delete pl2;
		delete pl3;
	};

	void runSimulation();

private:
	RLPlayer* pl1;//Function Approximation
	RLPlayer* pl2;//Rule Based
	RLPlayer* pl3;//Q-learning SARSA

};

#endif
