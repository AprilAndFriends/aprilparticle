#ifndef _LINEAR_DIRECTIONAL_FORCE_H_
#define _LINEAR_DIRECTIONAL_FORCE_H_

#include "DirectionalForceField.h"
#include "ParticleEmitter.h"

#include "gtypes/Vector3.h"

namespace April
{

	namespace Affectors
	{
		class aprilparticleExport LinearDirectionalForce : public Affector
		{

			public:
			
				gvec3 mDirection;
				float mForce;
			
				LinearDirectionalForce();
				LinearDirectionalForce(gvec3 direction = gvec3(0,-1,0), float force = 1.0);
				~LinearDirectionalForce();
				
				void draw();
				void update(Particle *particle, double t);

		};
		
	}

}

#endif // _LINEAR_DIRECTIONAL_FORCE_H_
