/*
 * Owner:Nuray Dindar, Ozgur Oguz
 * Summary:Q-learning and SARSA Controller of the game.
 * Date: March 04, 2011 
*/
#ifndef _RLQCONTROLLER_HPP_
#define _RLQCONTROLLER_HPP_

#include "RLController.hpp"
#include "../RLAction.hpp"
#include "../RLPlayer.hpp"

class RLQController : public RLController
{

	public:
		friend class RLPlayer;

		RLQController();
		~RLQController();
		RLQController(const RLQController &otherCtrl);
		RLQController & operator= (const RLQController &otherCtrl);

		// from abstract RLController class
		int decideAction(int step, double greedyProb, RLPlayer* p1, const RLPlayer* opp);
		void updateModel(RLPlayer*& p1, RLPlayer*& opp, int bestAct, int oppBestAct);
		double qvalue(double* ft);								

		double getQValues(int dist, int myAct, int oppAct) const { return qValues[dist][myAct][oppAct]; }
		double getVisits(int dist, int myAct, int oppAct) const { return visits[dist][myAct][oppAct]; }
		bool isSarsa() const { return sarsa; }
		void setSarsa(bool val) { sarsa = val; }

	private:
		double value(int dist, int oppAct);
		void copy(const RLQController& otherCtrl);
		void cleanup( void );

		double*** qValues;
		bool sarsa;
		int*** visits;
};
#endif