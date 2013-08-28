/*
 * Owner:Nuray Dindar, Ozgur Oguz
 * Summary:Players of the game.
 * Date: March 04, 2011 
*/
#ifndef _RLPLAYER_HPP_
#define _RLPLAYER_HPP_

#include "Controller/RLController.hpp"
#include "Controller/RLFAController.hpp"
#include "Controller/RLRBController.hpp"
#include "Controller/RLQController.hpp"
#include "RLAction.hpp"
#include <sstream>

class RLPlayer
{

public:

	friend class RLController;
	friend class RLAction;
	friend class RLQController;


	RLPlayer();
	RLPlayer(int* p, double h, int controllerType=0);
	RLPlayer(const RLPlayer &otherPlayer);
	RLPlayer & RLPlayer::operator=( const RLPlayer &otherPlayer );

	int makeAction(const RLPlayer* opp, double greedProb);

	~RLPlayer();

	RLAction* getAct() const { return act_; }
	void setAct(int act, int oXPos,bool setDis);


	RLAction* getPrevAct() const{ return prevAct_;}
	void setPrevAction(){ 
		delete prevAct_; prevAct_ = NULL; 
		prevAct_ = new RLAction(*act_);
	}

	// get-set pos
	int* getPos() const { return pos; };
	void setPos(int*& p) { 
		setPrevPos(pos);	// store previous position
		pos[0] = p[0];
		pos[1] = p[1];
	}
	// get/set previous position
	int* getPrevPos() const { return prevPos; };
	void setPrevPos(int*& p) { 
		prevPos[0] = p[0];
		prevPos[1] = p[1];
	}
	// get-set health
	double getHealth() const { return health; }
	void setHealth(double val) { health = val; }
	// get-set previous health
	double getPrevHealth() const { return prevHealth; }
	void setPrevHealth(double val) { prevHealth = val; }
	// get controller
	RLController* getController(){ return controller; };
	double updateHealth(RLAction* oppAct);
	string toString();
	int getHit() const { return isHit; }
	void setHit(int val) { isHit = val; }

private:
	RLController* controller;
	RLAction* act_;
	RLAction* prevAct_;

	int* pos;
	int* prevPos;
	double health;
	double prevHealth;
	int controllerType;

	void copy( const RLPlayer & otherPlayer );
	void cleanup( void );
	void updatePos(int dir);
	int isHit;

};

#endif