/*
 * Owner:Nuray Dindar, Ozgur Oguz
 * Summary: Players of the game.
 * Date: March 04, 2011 
*/
#include "../include/RLPlayer.hpp"

// Constructors
RLPlayer::RLPlayer(const RLPlayer &otherPlayer)
{
	copy(otherPlayer);
}

RLPlayer::RLPlayer()
{
	controller = new RLFAController;
	act_ = new RLAction;
	pos = new int[2];
	prevPos = new int[2];
	pos[0] = 0;
	pos[1] = 0;
	prevPos[0] = 0;
	prevPos[1] = 0;
	health = 100;
	prevHealth = 100;
	isHit = -1;
}

RLPlayer::RLPlayer(int* p, double h, int controllerType)
{
	this->controllerType = controllerType;

	if (controllerType==0)
		controller = new RLFAController;
	else if (controllerType==1)
		controller = new RLRBController;
	else if (controllerType==2)
		controller = new RLQController;
	act_ = new RLAction;
	prevAct_ = new RLAction;

	pos = new int[2];
	pos[0] = p[0];
	pos[1] = p[1];
	prevPos = new int[2];
	prevPos[0] = p[0];
	prevPos[1] = p[1];
	health = h;
	prevHealth = health;
	isHit = -1;
}

// Deconstructor
RLPlayer::~RLPlayer()
{
	cleanup();
}

// Helpers
void RLPlayer::copy(const RLPlayer & otherPlayer)
{
	pos = new int[2];
	prevPos = new int[2];
	controllerType = otherPlayer.controllerType;
	if(controllerType==0){
		const RLFAController* c1 = static_cast<RLFAController*>(otherPlayer.controller);
		controller = new RLFAController(*c1);
	}else if(controllerType==1){
		const RLRBController* c1 = static_cast<RLRBController*>(otherPlayer.controller);
		controller = new RLRBController(*c1);
	}else if(controllerType==2){
		const RLQController* c1 = static_cast<RLQController*>(otherPlayer.controller);
		controller = new RLQController(*c1);
	}
	act_ = new RLAction(*otherPlayer.act_);
	prevAct_ = new RLAction(*otherPlayer.prevAct_);

	pos[0] = otherPlayer.pos[0];
	pos[1] = otherPlayer.pos[1];
	prevPos[0] = otherPlayer.prevPos[0];
	prevPos[1] = otherPlayer.prevPos[1];

	health = otherPlayer.health;
	prevHealth = otherPlayer.prevHealth;
	isHit = otherPlayer.isHit;
}

void RLPlayer::cleanup(void)
{
	delete controller;
	delete act_;
	delete pos;
	delete prevPos;
	delete prevAct_;

	controller = NULL;
	act_ = NULL;
	pos = NULL;
	prevPos = NULL;
	prevAct_ = NULL;
}

RLPlayer & RLPlayer::operator=(const RLPlayer &otherPlayer)
{
	if (&otherPlayer != this ) {
		cleanup();
		copy( otherPlayer );
	}
	return *this;
}

// Update the health of players and return the reward based on his action
double RLPlayer::updateHealth(RLAction* oppAct)
{
	// store previous health
	setPrevHealth(this->health);

	int o_ActType = oppAct->getType();
	double oActVal = oppAct->getValue(oppAct->getAction(), this->act_->getDist());
	double myActVal = this->act_->getValue(this->act_->getAction(), this->act_->getDist());
	double rwd = 0;
if (o_ActType == RLAction::OFF)
	{
		if (act_->getType() == RLAction::OFF)
		{
			this->health -= oActVal;
			// reward is the difference between damages (notice, it could be negative (punishment))
			if(myActVal>oActVal)
				rwd = myActVal;
			else if (oActVal>myActVal)
				rwd = -oActVal;
		}
		else if (act_->getType() == RLAction::NEU)
		{
			if (oActVal>0)					// if I took damage
			{	
				this->health -= oActVal;
			}
		}
		else if (act_->getType() == RLAction::DEF){
			this->health -= (oActVal - myActVal);
			rwd = -oActVal + myActVal;		// I defended myself but I still took some reduced damage
		}
	}
	else if (o_ActType == RLAction::DEF)
	{
		if (act_->getType()==RLAction::OFF)	// if I attacked while opponent defended itself
		{
			if (myActVal>0)					// and if I made damage
				rwd = myActVal - oActVal;		// then I'm rewarded by the amount of that damage
			else							// if I couldn't hit
				rwd = myActVal;				// then no reward or punishment
		}
		// WARNING-Resolved: consider walking/running backward&forward
	}
	else if (o_ActType == RLAction::NEU)
	{
		if (act_->getType()==RLAction::OFF){	// if I attack while my opponent acts neutrally
			if (myActVal>0)				// and if I make damage 
				rwd = myActVal;			// then I'm awarded by the amount of that damage
			else						// if I couldn't make damage
				rwd = -oActVal;			// then I'm punished by the amount of my opponent's action value
		}			
		else if (act_->getType()==RLAction::DEF){	// if I defend while my opponent acts neutrally
			rwd = -oActVal;							// i'm punished
		}
	}
	
	return rwd;
}

// Set player's action 
void RLPlayer::setAct(int act, int oXPos ,bool setDis)
{ 
	setPrevAction();

	act_->setAction(act);
	int actType = act_->getType();
	int dir = oXPos-getPos()[0];

	if (dir==0)
		dir = 1;

	dir /= abs(dir);

	if ( actType == RLAction::NEU)
		updatePos(dir);

	dir = oXPos-getPos()[0];
	if(setDis)
		act_->setDist(abs(dir));
}

// Update player's position based on the action
void RLPlayer::updatePos(int dir)
{
	setPrevPos(pos);
	if (act_->getAction() == RLAction::WALK_F)
		pos[0] += dir*1;
	else if (act_->getAction() == RLAction::WALK_B)
		pos[0] -= dir*1;
	else if (act_->getAction() == RLAction::RUN_F)
		pos[0] += dir*2;
	else if (act_->getAction() == RLAction::RUN_B)
		pos[0] -= dir*2;
}

int RLPlayer::makeAction(const RLPlayer* opp, double greedProb)
{
	int bestAct = controller->decideAction(1, greedProb , this, opp);
	return bestAct;
}

// toString method
std::string RLPlayer::toString()
{
	stringstream out;

	out<< " x = " << pos[0] << "\ty = " << pos[1];
	out<< "\tH = " << health ;
	out<< "\tA = " << act_->getAction() << "\tD = " << act_->getDist()<<endl;

	return out.str();
}