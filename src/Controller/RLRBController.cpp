/*
 * Owner:Nuray Dindar, Ozgur Oguz
 * Summary:Rule Based Controller of the game. This controller isn't fully implemented.
 * Date: March 04, 2011 
*/
#include "../../include/Controller/RLRBController.hpp"

int RLRBController::decideAction(int step, double greedyProb, RLPlayer* p1, const RLPlayer* opp)
{
	double rnd = (double) rand()/RAND_MAX;
	int dis = p1->getAct()->getDist();
	int bestAct = -1;

	if(dis == RLAction::CLOSE) 
	{
		if(rnd < 0.3)
			bestAct = RLAction::U_CUT;
		else if (rnd < 0.7)
			bestAct = RLAction::WALK_B;
		else 
			bestAct = RLAction::BLOCK;
	}
	else if(dis == RLAction::NEAR)
	{
		if(rnd < 0.4)
			bestAct = RLAction::L_KICK;
		else
			bestAct = RLAction::WALK_F;
	}
	else
	{
		if(rnd < 0.5)
			bestAct = RLAction::RUN_F;
		else
			bestAct = RLAction::WALK_F;
	}
	
	return bestAct;
}

double RLRBController::qvalue(double* ft)
{
	double qTotal = 0;
	/*for(int i=0; i< NUM_OF_FEATS; i++)
		qTotal += feat->getFeatWeight()[i]*ft[i];*/
	return qTotal;
}
// do-step in FAController
void RLRBController::updateModel(RLPlayer*& p1, RLPlayer*& opp, int bestAct, int oppBestAct)
{
	checkBorder(p1);
	int m_dist = abs(opp->getPos()[0] - p1->getPos()[0]);
	p1->getAct()->setDist(m_dist);
	double rwd = p1->updateHealth(opp->getAct());
}

RLRBController::RLRBController() /*: RLController()*/
{
	//feat = new RLFeature;
}

RLRBController::RLRBController(const RLRBController &otherCtrl)
{
	copy(otherCtrl);
}

RLRBController & RLRBController::operator=(const RLRBController &otherCtrl)
{
	if ( &otherCtrl != this ) {
		cleanup();
		copy( otherCtrl );
	}
	return *this;

}

void RLRBController::copy(const RLRBController& otherCtrl)
{
	setReward(otherCtrl.getReward());
	setPrevReward(otherCtrl.getPrevReward());
}

void RLRBController::cleanup(void)
{

}


