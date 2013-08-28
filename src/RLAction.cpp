/*
 * Owner:Nuray Dindar, Ozgur Oguz
 * Summary: Action class provides the action performed by the player such as kick, upper cut, walk forward,...
 * Date: March 04, 2011 
*/
#include "../include/RLAction.hpp"

double** RLAction::value;

// Constructors
RLAction::RLAction(): action(L_KICK), dist(0), type(0)
{
	prevDist = 0;
	setVars();
}

RLAction::RLAction( const RLAction &otherAct )
{
	action = otherAct.action;
	type = otherAct.type;
	dist = otherAct.dist;
	prevDist = otherAct.prevDist;
}

// Setters
void RLAction::setAction(int val) 
{ 
	action = val; 
	if (action<=U_CUT)
		setType(OFF);
	else if (action<=BLOCK)
		setType(DEF);
	else if (action<=RUN_B)
		setType(NEU);
}

void RLAction::setDist(int val) 
{ 
	setPrevDist();
	if ( val < 2 )
		dist = CLOSE;
	else if ( val < 3 )
		dist = NEAR;
	else dist = FAR;
}

void RLAction::setPrevDist() 
{ 
	prevDist = dist;
}

void RLAction::setVars()
{
	value = new double*[ACTION_COUNT];
	for (int i=0; i<ACTION_COUNT; i++)
	{
		value[i] = new double[3];
		if(i==L_KICK)
			setVals(i, 7, 10, 0);
		else if(i==U_CUT)
			setVals(i, 15, 7, 0);
		else if(i==BLOCK)
			setVals(i, 3, 4, 0);
		else if(i==WALK_F)
			setVals(i, 0, 3, 4);
		else if(i==WALK_B)
			setVals(i, 5, 8, 0);
		else if(i==RUN_F)
			setVals(i, 0, 0, 4);
		else if(i==RUN_B)
			setVals(i, 5, 8, 0);
	}

}

void RLAction::setVals(int act, double dc, double dn, double df)
{
	value[act][0] = dc;
	value[act][1] = dn;
	value[act][2] = df;
}