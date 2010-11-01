#ifndef _DIRECTIONAL_FORCE_FIELD_H_
#define _DIRECTIONAL_FORCE_FIELD_H_

#include "gtypes/Vector3.h"
#include "Particle.h"
#include "Affector.h"

namespace April
{
	namespace Affectors
	{

		class aprilparticleExport DirectionalForceField : public Affector
		{

			public:
				float mForce;
				gvec3 mPosition;
				gvec3 mDirection;
			
				DirectionalForceField();
				DirectionalForceField(float force, gvec3 position, gvec3 direction);
				~DirectionalForceField();
				
				void update(Particle *particle, double t);
				void draw();

		};
		
	}

}

#endif // _DIRECTIONAL_FORCE_FIELD_H_
