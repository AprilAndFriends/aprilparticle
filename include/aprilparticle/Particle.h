#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "aprilparticleExport.h"
#include "gtypes/Vector3.h"

namespace April
{
	class aprilparticleExport Particle
	{

		public:
		
			gvec3 				mPosition, mDirection;
			
			float 				mLife, mTotalLife;
			float 				mSize;
			float 				mSpeed;
			float 				mAngle;
			
			unsigned int mColor;
		
			Particle();
			Particle(float life, gvec3 position, gvec3 direction, float size);
	};

}

#endif // _PARTICLE_H_
