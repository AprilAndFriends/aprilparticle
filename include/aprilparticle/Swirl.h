#ifndef _SWIRL_H_
#define _SWIRL_H_

#include "aprilparticleExport.h"
#include "Affector.h"
#include "Particle.h"
#include "gtypes/Matrix3.h"

namespace April
{

	namespace Affectors
	{

		class aprilparticleExport Swirl : public Affector
		{
			gmat3 _mRotation;

		public:
		
			float mRange, mForce;
			gvec3 mPosition, mUp;
			bool mClockwise;
			
			Swirl();
			Swirl(gvec3 position, gvec3 up, float range, float force);
			~Swirl();
			
			void draw();
			void update(April::Particle *particle, double t);

		};

	}

}

#endif // _SWIRL_H_
