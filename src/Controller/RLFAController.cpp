/*
 * Owner:Nuray Dindar, Ozgur Oguz
 * Summary:Function Approximation Controller of the game.
 * Date: March 04, 2011 
*/
#include "../../include/Controller/RLFAController.hpp"

int RLFAController::decideAction(int step, double greedyProb, RLPlayer* p1, const RLPlayer* opp)
{
	double rnd = (double)rand()/RAND_MAX;

	int bestAct = -1;
	RLPlayer* pl1 = new RLPlayer(*p1);
	RLPlayer* pl2 = new RLPlayer(*opp);
	
	if (rnd < greedyProb)
	{
		int startAct = rand()%RLAction::ACTION_COUNT;
		pl1->setAct(startAct, pl2->getPos()[0],false);
		checkBorder(pl1);
		int m_dist = abs(pl2->getPos()[0] - pl1->getPos()[0]);
		pl1->getAct()->setDist(m_dist);
		pl2->getAct()->setDist(m_dist);

		pl1->updateHealth(pl2->getAct());
		pl2->updateHealth(pl1->getAct());
		// compute new feature values
		feat->setFeats(pl1, pl2);

		bestAct = startAct;

		double bestQValue = qvalue(feat->getFeats());
		double tempQValue = 0;

		for (int i=1; i<RLAction::ACTION_COUNT; i++)
		{
			startAct = (startAct+1)%RLAction::ACTION_COUNT;
			delete pl1, pl2;
			pl1 = new RLPlayer(*p1);
			pl2 = new RLPlayer(*opp);
		
			pl1->setAct(startAct, pl2->getPos()[0],false);
			checkBorder(pl1);

			int m_dist = abs(pl2->getPos()[0] - pl1->getPos()[0]);
			pl1->getAct()->setDist(m_dist);
			pl2->getAct()->setDist(m_dist);

			pl1->updateHealth(pl2->getAct());
			pl2->updateHealth(pl1->getAct());
			// compute new feature values
			feat->setFeats(pl1, pl2);
			tempQValue = qvalue(feat->getFeats());
			if(bestQValue < tempQValue ){
				bestAct = startAct;
				bestQValue = tempQValue;
			}			
		} 
	}
	else{
		bestAct = rand()%RLAction::ACTION_COUNT;
	}
	
	delete pl1;
	delete pl2;
	pl1 = NULL;
	pl2 = NULL;

	return bestAct;
}

double RLFAController::qvalue(double* ft)
{
	double qTotal = 0;
	for(int i=0; i< NUM_OF_FEATS; i++)
		qTotal += feat->getFeatWeight()[i]*ft[i];

	return qTotal;
}
// do-step in FAController
void RLFAController::updateModel(RLPlayer*& p1, RLPlayer*& opp, int bestAct, int oppBestAct)
{
	p1->setAct(bestAct, opp->getPos()[0], false); //nuray
	opp->setAct(oppBestAct, p1->getPos()[0],false);

	double wallPun = checkBorder(p1);
	checkBorder(opp);

	int m_dist = abs(opp->getPos()[0] - p1->getPos()[0]);
	p1->getAct()->setDist(m_dist);
	opp->getAct()->setDist(m_dist);

		 
	double rwd = p1->updateHealth(opp->getAct());
	opp->updateHealth(p1->getAct());

	// compute new feature values
	feat->setFeats(p1, opp);

	// compute delta by first computing q-values
	double delta = getPrevReward()+ getDiscount()*qvalue(feat->getFeats()) - qvalue(feat->getPrevFeat());

	// update feature weights
	double* newW = new double[NUM_OF_FEATS];
	for(int i=0; i< NUM_OF_FEATS; i++)
		newW[i] = feat->getFeatWeight()[i] + getAlpha()*delta*feat->getPrevFeat()[i];

	feat->setFeatWeight(newW);
		
	feat->setPrevFeat(feat->getFeats());

	// set reward. add up if the player hits the walls
	setPrevReward(rwd+wallPun);	

	delete newW;
	newW = NULL;
}
// Constructors
RLFAController::RLFAController()
{
	feat = new RLFeature;
}

RLFAController::RLFAController(const RLFAController &otherCtrl)
{
	copy(otherCtrl);
}

RLFAController & RLFAController::operator=(const RLFAController &otherCtrl)
{
	if ( &otherCtrl != this ) {
		cleanup();
		copy( otherCtrl );
	}
	return *this;
}

void RLFAController::copy(const RLFAController& otherCtrl)
{
	const RLFeature* c1 = static_cast<RLFeature*>(otherCtrl.feat);
	feat = new RLFeature(*c1);
	setReward(otherCtrl.getReward());
	setPrevReward(otherCtrl.getPrevReward());
}
// Deconstructor
RLFAController::~RLFAController(void)
{
	cleanup();
}

void RLFAController::cleanup(void)
{
	allReward.clear();
	delete feat;
	feat = NULL;
}

RLFeature* RLFAController::getFeats()
{
	return feat;
}
