/*
 * Owner:Nuray Dindar, Ozgur Oguz
 * Summary: Function Approximation Controller of the game.
 * Date: March 04, 2011 
*/
#ifndef _RLFACONTROLLER_HPP_
#define _RLFACONTROLLER_HPP_

#include "RLController.hpp"
#include "../RLFeature.hpp"

class RLFAController : public RLController
{
	public:
		friend class RLFeature;

		RLFAController();
		RLFAController(const RLFAController &otherCtrl);
		RLFAController & operator= (const RLFAController &otherCtrl);
		~RLFAController();

		// from abstract RLController class
		int decideAction(int step, double greedyProb, RLPlayer* p1, const RLPlayer* opp);	
		// from abstract RLController class
		double qvalue(double* ft);							
		RLFeature* getFeats();

		void updateModel( RLPlayer*& p1, RLPlayer*& opp, int bestAct, int oppBestAct );

	private:
		RLFeature* feat;

		void copy( const RLFAController& otherCtrl );
		void cleanup( void );
};
#endif