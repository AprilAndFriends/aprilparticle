#include "LinearDirectionalForce.h"

namespace April
{
	
	namespace Affectors
	{
		
		LinearDirectionalForce::LinearDirectionalForce()
		{
			mDirection 			= gvec3(0,-1,0);
			mForce 				= 1.0;
		}
		
		LinearDirectionalForce::LinearDirectionalForce(gvec3 direction, float force)
		{
			mDirection 			= direction;
			mForce 				= force;
		}

		LinearDirectionalForce::~LinearDirectionalForce()
		{
		}
		
		void LinearDirectionalForce::draw()
		{
			
		}
		
		void LinearDirectionalForce::update(Particle *particle, double t)
		{
			particle->mSpeed += mForce * t;
			particle->mPosition += mDirection * particle->mSpeed * particle->mDirection.length();
		}
		
	}

}

