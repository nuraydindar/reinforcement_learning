#include "../include/RLSimulator.h"
#include <fstream>
/*
 * Owner:Nuray Dindar, Ozgur Oguz
 * Summary: Simulation of the game. Set the type of reinforcement learning you would like to use for the controller
 *			RLFA: Reinforcement Learning with Function Approximation
 *			RLRB(not define RLFA): Reinforcement Learning with Rule Based Agents (SARSA, Q-learning)
 * Date: March 04, 2011 
*/
#define RLFA

// Constructor
RLSimulator::RLSimulator(void)
{
	int p1pos[2] = {5,0};
	int p2pos[2] = {6,0};
	pl1 = new RLPlayer(p1pos, 200000, 0);
	pl2 = new RLPlayer(p2pos, 200000, 1);
	pl3 = new RLPlayer(p1pos, 200000, 2);
	// initialize the features for the FA controlled player 
	((RLFAController*)(pl1->getController()))->getFeats()->setFeats(pl1, pl2);
}

void RLSimulator::runSimulation()
{
	for( int k=0 ; k<10; k++){
		
		int p1pos[2] = {5,0};
		int p2pos[2] = {6,0};
#ifdef RLFA		
		pl2 = new RLPlayer(p2pos, 200000, 1);
		pl1 = new RLPlayer(p1pos, 200000, 0);
#endif
		int step = 20000;

		double greedyProb = 0.8;

#ifndef RLFA
		pl2 = new RLPlayer(p2pos, 200000, 1);
		pl3 = new RLPlayer(p1pos, 200000, 2);
		RLPlayer * pl4 = new RLPlayer(*pl2);
#endif

		for ( int i=0; i<step; i++ )
		{
			if(i>step/2)
				greedyProb = 1;
#ifdef RLFA 

			// store first players state
			RLPlayer* prevP1 = new RLPlayer(*pl1);
			// player 2 select its action based on predefined rules 
			int bestAct2 = pl2->makeAction(prevP1,greedyProb);
			pl2->setAct(bestAct2, pl1->getPos()[0],false);
			((RLRBController*)(pl2->getController()))->checkBorder(pl2);
			// player 1 select its action based on the percentage of greedy exploitation
			int bestAct1 = pl1->makeAction(pl2,greedyProb);

			((RLFAController*)(pl1->getController()))->updateModel(pl1,pl2, bestAct1,bestAct2);		

			delete prevP1;
			prevP1 = NULL;
			
#endif
#ifndef RLFA 
			int bestAct4 = pl4->makeAction(pl3,greedyProb);
			pl4->setAct(bestAct4, pl3->getPos()[0],false);// sets only the action doesn't update the pos and distance
			((RLRBController*)(pl4->getController()))->checkBorder(pl4);

			int bestAct3 = pl3->makeAction(pl4,greedyProb);
			((RLQController*)(pl3->getController()))->updateModel(pl3,pl4, bestAct3,bestAct4);

#endif
		}
#ifndef RLFA 
		cout<< "Player SA : "<< pl3->toString();
		cout<< "Player RB : "<< pl4->toString();
		delete pl4; pl4 = NULL;

		ofstream myfile;
		char name[250];
		sprintf(name,"data/RewardsQ%d.txt",k);
		myfile.open (name);
		vector<double> rewards = pl3->getController()->getAllReward();
		for( int i = 0; i< rewards.size(); i++ )
			myfile <<rewards.at(i) << endl;
		myfile.close();
		delete pl2, pl3;
		pl3 = NULL; 
		pl2 = NULL;
#endif

#ifdef RLFA 
		cout<< "Player FA : "<< pl1->toString();
		cout<< "Player RB : "<< pl2->toString();
		ofstream myfile;
		char name[250];
		sprintf(name,"data/RewardsFN%d.txt",k);
		myfile.open (name);
		vector<double> rewards = pl1->getController()->getAllReward();
		for( int i = 0; i< rewards.size(); i++ )
			myfile <<rewards.at(i) << endl;
		myfile.close();
		delete pl2, pl1;
		pl1 = NULL; 
		pl2 = NULL;
#endif	
	}
}

void main()
{
	RLSimulator* sim = new RLSimulator;
	sim->runSimulation();
}
