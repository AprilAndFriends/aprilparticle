#ifndef DIRECTIONAL_FORCE_FIELD_H
#define DIRECTIONAL_FORCE_FIELD_H

#include <gtypes/Vector3.h>

#include "Affector.h"

namespace april
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport DirectionalForceField : public Affector
		{
		public:
			DirectionalForceField();
			DirectionalForceField(float force, gvec3 position, gvec3 direction);
			~DirectionalForceField();
				
			void update(Particle* article, double t);
			void draw();

		protected:
			gvec3 position;
			gvec3 direction;
			float force;
			
		};
		
	}

}

#endif
