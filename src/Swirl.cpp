#include "Swirl.h"
#include <math.h>

namespace April
{

	namespace Affectors
	{

		Swirl::Swirl()
		{
			mClockwise 		= true;
			mPosition 		= gvec3(0.0,0.0,0.0);
			mUp 			= gvec3(0.0,1.0,0.0);
			mRange 			= 1.0;
			mForce 			= 1.0;
			
			_mRotation.setIdentity();
		}

		Swirl::~Swirl()
		{
		}
		
		Swirl::Swirl(gvec3 position, gvec3 up, float range, float force)
		{
			mClockwise 		= true;
			mPosition 		= position;
			mUp 			= up;
			mRange 			= range;
			mForce 			= force;
			
			_mRotation.setIdentity();
		}
		
		void Swirl::draw()
		{
			
		}
		
		void Swirl::update(April::Particle* particle, double t)
		{
			gvec3 direction = (mPosition - particle->mPosition);
			float sqlen = direction.squaredLength();
			if(sqlen < mRange * mRange * 0.25)
			{
				float clockwise;
				mClockwise ? clockwise = -1.0 : clockwise = 1.0;
				_mRotation.setRotation3D(mUp, (1.0 - sqrt(sqlen) / mRange) * mForce * t * clockwise * 360.0);
				gvec3 pos;
				pos = particle->mPosition - mPosition;
				particle->mPosition = mPosition + _mRotation * pos;
			}
		}

	}

}

