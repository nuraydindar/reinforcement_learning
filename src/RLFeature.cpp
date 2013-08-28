/*
 * Owner:Nuray Dindar, Ozgur Oguz
 * Summary: Configure the features for the game.
 * Date: March 04, 2011 
*/
#include "../include/Controller/RLFAController.hpp"

// Constructors
RLFeature::RLFeature(const RLPlayer* p1, const RLPlayer* opp)
{
	initializeFeat();
	setFeats(p1, opp);
}

RLFeature::RLFeature()
{
	feats = new double[NUM_OF_FEATS];
	prevFeats = new double[NUM_OF_FEATS];
	featWeight = new double[NUM_OF_FEATS];
	initializeFeat();
}

RLFeature::RLFeature(const RLFeature &otherFeat)
{
	feats = new double[NUM_OF_FEATS];
	prevFeats = new double[NUM_OF_FEATS];
	featWeight = new double[NUM_OF_FEATS];

	for(int i=0; i<NUM_OF_FEATS; i++)
	{
		feats[i] = otherFeat.feats[i];
		prevFeats[i] = otherFeat.prevFeats[i];
		featWeight[i] = otherFeat.featWeight[i];
	}
}

// Initialization
void RLFeature::initializeFeat()
{
	for(int i=0; i<NUM_OF_FEATS; i++)
	{
		prevFeats[i] = 0;
		featWeight[i] = 1;
	}
}

// Set Features
void RLFeature::setFeats(const RLPlayer* p1, const RLPlayer* opp)
{
	double healthDiffP1 = (p1->getPrevHealth()-p1->getHealth());
	double healthDiffOp = (opp->getPrevHealth()-opp->getHealth());
	
	// health
	feats[0] = (healthDiffP1 - healthDiffOp)/5.0; 
	
	// distance in x-direction
	feats[1] = (double)(p1->getPos()[0] - opp->getPos()[0])/5.0;
	
	// distance in y-direction
	feats[2] = p1->getAct()->getValue(p1->getAct()->getAction(), p1->getAct()->getDist())/5.0;
	
	// hitting the wall
	feats[3] = (double)p1->getHit()/2.0;
	feats[4] = p1->getAct()->getDist()+1;

	// damage
	feats[5] = opp->getAct()->getValue(opp->getAct()->getAction(), opp->getAct()->getDist())/5.0;
}