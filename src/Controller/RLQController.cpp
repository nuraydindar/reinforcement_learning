/*
 * Owner:Nuray Dindar, Ozgur Oguz
 * Summary:Q-learning and SARSA Controller of the game.
 * Date: March 04, 2011 
*/
#include "../../include/Controller/RLQController.hpp"

// Constructor
RLQController::RLQController(void)
{
	qValues = new double**[RLAction::DIST_COUNT];
	visits = new int**[RLAction::DIST_COUNT];
	for (int i=0; i<RLAction::DIST_COUNT; i++)
	{
		qValues[i] = new double*[RLAction::ACTION_COUNT];
		visits[i] = new int*[RLAction::ACTION_COUNT];
		for (int j=0; j<RLAction::ACTION_COUNT; j++) 
		{
			qValues[i][j] = new double[RLAction::ACTION_COUNT];
			visits[i][j] = new int[RLAction::ACTION_COUNT];
			for (int k=0; k<RLAction::ACTION_COUNT; k++)
			{
				qValues[i][j][k] = 0;
				visits[i][j][k] = 0;
			}
		}
	}
	sarsa = false;
}

// Deconstructor
RLQController::~RLQController(void)
{
	cleanup();
}

void RLQController::cleanup(void)
{
	delete [] qValues;
	delete [] visits;
	qValues = NULL; 
	visits = NULL;
}

RLQController & RLQController::operator=(const RLQController &otherCtrl)
{
	if ( &otherCtrl != this ) {
		cleanup();
		copy( otherCtrl );
	}
	return *this;
}

// Copy Constructor
RLQController::RLQController(const RLQController &otherCtrl)
{
	copy(otherCtrl);
}

void RLQController::copy(const RLQController& otherCtrl)
{
	qValues = new double**[RLAction::DIST_COUNT];
	visits = new int**[RLAction::DIST_COUNT];
	for (int i=0; i<RLAction::DIST_COUNT; i++)
	{
		qValues[i] = new double*[RLAction::ACTION_COUNT];
		visits[i] = new int*[RLAction::ACTION_COUNT];
		for (int j=0; j<RLAction::ACTION_COUNT; j++) 
		{
			qValues[i][j] = new double[RLAction::ACTION_COUNT];
			visits[i][j] = new int[RLAction::ACTION_COUNT];
			for (int k=0; k<RLAction::ACTION_COUNT; k++)
			{
				qValues[i][j][k] = otherCtrl.qValues[i][j][k];
				visits[i][j][k] =  otherCtrl.visits[i][j][k];
			}
		}
	}
	sarsa = otherCtrl.sarsa;
}

// Decide the actions based on q-learning
int RLQController::decideAction(int step, double greedyProb, RLPlayer* p1, const RLPlayer* opp)
{
	double rnd = (double)rand()/RAND_MAX;

	int bestAct = -1;
	RLPlayer* pl1 = new RLPlayer(*p1);
	RLPlayer* pl2 = new RLPlayer(*opp);
	//WARNING : We assume that the best action of opponent is known at current time
	if (rnd < greedyProb)
	{
		// compute new feature values
		int startAct = rand()%RLAction::ACTION_COUNT;
		bestAct = startAct;
		double bestQValue = getQValues(pl1->getAct()->getDist(), startAct, pl2->getAct()->getAction());
		double tempQValue = 0;

		for (int i=1; i<RLAction::ACTION_COUNT; i++)
		{
			startAct = (startAct+1)%RLAction::ACTION_COUNT;
			tempQValue = getQValues(pl1->getAct()->getDist(),startAct, pl2->getAct()->getAction());

			if(bestQValue < tempQValue ){
				bestAct = startAct;
				bestQValue = tempQValue;
			}
		}
	}
	else{
		bestAct = rand()%RLAction::ACTION_COUNT;
	}

	delete pl1;	delete pl2;
	pl1 = NULL;	pl2 = NULL;

	return bestAct;
}
void RLQController::updateModel( RLPlayer*& p1, RLPlayer*& opp, int bestAct, int oppBestAct)
{
	// sarsa learning
	if (isSarsa())
	{
		RLPlayer* pl1 = new RLPlayer(*p1);
		RLPlayer* pl2 = new RLPlayer(*opp);

		// get previous distance between players
		int p_d = p1->getPrevAct()->getPrevDist();
		// get first player's action
		int p_a1 = p1->getAct()->getAction();	
		// get second player's action
		int p_a2 = opp->getPrevAct()->getAction();	

		// get previous distance between players
		int d = pl1->getAct()->getDist();
		int a1 = bestAct;	
		int a2 = oppBestAct; 

		double newDatum = getPrevReward() + getDiscount()*getQValues(d, a1, a2);
		// update visits on that state
		visits[p_d][p_a1][p_a2]++;	
		setAlpha(1.0/visits[p_d][p_a1][p_a2]);

		qValues[p_d][p_a1][p_a2] = (1-getAlpha())*(qValues[p_d][p_a1][p_a2]) + getAlpha()*newDatum;

		p1->setAct(bestAct, opp->getPos()[0],false);
		double wallPun = checkBorder(p1);

		int m_dist = abs(opp->getPos()[0] - p1->getPos()[0]);
		p1->getAct()->setDist(m_dist);
		opp->getAct()->setDist(m_dist);

		double rwd = p1->updateHealth(opp->getAct());
		opp->updateHealth(p1->getAct());
		// set reward. add up if the player hits the walls
		setPrevReward(rwd+wallPun);	
		delete pl1;	delete pl2;
		pl1 = NULL;	pl2 = NULL;
	}
	//Q-learning
	else
	{ 
		p1->setAct(bestAct, opp->getPos()[0],false);

		double wallPun = checkBorder(p1);

		int m_dist = abs(opp->getPos()[0] - p1->getPos()[0]);
		p1->getAct()->setDist(m_dist);
		opp->getAct()->setDist(m_dist);

		RLPlayer* pl1 = new RLPlayer(*p1);
		RLPlayer* pl2 = new RLPlayer(*opp);

		// get previous distance between players
		int p_d = pl1->getPrevAct()->getDist();		

		double rwd = wallPun + p1->updateHealth(opp->getAct());
		opp->updateHealth(p1->getAct());
		
		double newDatum=rwd+getDiscount()*value(p1->getAct()->getDist(),oppBestAct);
		// update visits on that state
		visits[p_d][bestAct][oppBestAct]++;

		qValues[p_d][bestAct][oppBestAct] = (1-getAlpha())*(qValues[p_d][bestAct][oppBestAct]) + getAlpha()*newDatum;

		setPrevReward(rwd);	
		delete pl1;	delete pl2;
		pl1 = NULL;	pl2 = NULL;
	}

}

double RLQController::value(int dist, int oppAct){

	double val = getQValues(dist, RLAction::ACTION_COUNT-1, oppAct);

	for(int i=RLAction::ACTION_COUNT-2; i>=0; i--)
	{
		if(getQValues(dist, i,oppAct)>val)
			val = getQValues(dist, i,oppAct);
	}
	return val;
}

double RLQController::qvalue(double* ft)
{
	return 0;
}
