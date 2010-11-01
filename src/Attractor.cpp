#include "Attractor.h"

namespace April
{

	namespace Affectors
	{

		Attractor::Attractor()
		{
		}
		
		Attractor::Attractor(gvec3 origin, float radius)
		{
			mOrigin = origin;
			mRadius = radius;
		}

		Attractor::~Attractor()
		{
		}
		
		void Attractor::draw()
		{
		}
		
		void Attractor::update(Particle *particle, double t)
		{
			gvec3 mDirection(mOrigin - particle->mPosition);
			//mDirection.normalise();
			float len = (particle->mPosition - mOrigin).length();
			float lensquared = (particle->mPosition - mOrigin).squaredLength();
			particle->mPosition += mDirection * (mRadius - (len) / mRadius) *
								   (mRadius / (lensquared + 1.0)) * t / mRadius;
		}

	}

}

