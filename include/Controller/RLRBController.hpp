/*
 * Owner:Nuray Dindar, Ozgur Oguz
 * Summary:Rule based controller of the game.
 * Date: March 04, 2011 
*/
#ifndef _RLRBCONTROLLER_HPP_
#define _RLRBCONTROLLER_HPP_

#include "RLController.hpp"
#include "../RLFeature.hpp"

class RLRBController : public RLController
{
	public:
		friend class RLFeature;

		RLRBController();
		RLRBController(const RLRBController &otherCtrl);
		RLRBController & operator= (const RLRBController &otherCtrl);

		// from abstract RLController clas
		int decideAction(int step, double greedyProb, RLPlayer* p1, const RLPlayer* opp);
		double qvalue(double* ft);					
	
		RLFeature* getFeats();
		void updateModel(RLPlayer*& p1, RLPlayer*& opp, int bestAct, int oppBestAct);

	private:
		void copy(const RLRBController& otherCtrl );
		void cleanup( void );
};
#endif