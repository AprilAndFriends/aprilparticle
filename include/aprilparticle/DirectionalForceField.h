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
			DirectionalForceField(gvec3 position, gvec3 direction, float force);
			~DirectionalForceField();

			gvec3 getPosition() { return this->position; }
			void setPosition(gvec3 value) { this->position = value; }
			gvec3 getDirection() { return this->direction; }
			void setDirection(gvec3 value) { this->direction = value; }
			float getForce() { return this->force; }
			void setForce(float value) { this->force = value; }
				
			void update(Particle* article, float k);
			void draw();

		protected:
			gvec3 position;
			gvec3 direction;
			float force;
			
		};
		
	}

}

#endif
