#ifndef LINEAR_DIRECTIONAL_FORCE_H
#define LINEAR_DIRECTIONAL_FORCE_H

#include <gtypes/Vector3.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	class aprilparticleExport LinearDirectionalForce : public Affector
	{
	public:
		LinearDirectionalForce(gvec3 direction = gvec3(0.0f, -1.0f, 0.0f), float force = 1.0f);
		~LinearDirectionalForce();
		
		gvec3 getDirection() { return this->direction; }
		void setDirection(gvec3 value) { this->direction = value; }
		float getForce() { return this->force; }
		void setForce(float value) { this->force = value; }

		void update(Particle* particle, float k);

	protected:
		gvec3 direction;
		float force;
	
	};

}

#endif
