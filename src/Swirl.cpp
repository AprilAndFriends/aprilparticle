#include <math.h>

#include "Particle.h"
#include "Swirl.h"

namespace april
{
	namespace Affectors
	{
		Swirl::Swirl()
		{
			this->_rotation.setIdentity();
			this->position = gvec3(0.0f, 0.0f, 0.0f);
			this->up = gvec3(0.0f, 1.0f, 0.0f);
			this->range = 1.0f;
			this->force = 1.0f;
			this->clockwise = true;
		}

		Swirl::Swirl(gvec3 position, gvec3 up, float range, float force)
		{
			this->_rotation.setIdentity();
			this->position = position;
			this->up = up;
			this->range = range;
			this->force = force;
			this->clockwise = true;
		}
		
		Swirl::~Swirl()
		{
		}
		
		void Swirl::update(april::Particle* particle, float k)
		{
			gvec3 position = particle->getPosition();
			gvec3 direction = (this->position - position);
			float sqlen = direction.squaredLength();
			if (sqlen < this->range * this->range * 0.25)
			{
				float clockDirection = (this->clockwise ? -1.0f : 1.0f);
				this->_rotation.setRotation3D(this->up, (1.0f - sqrt(sqlen) / this->range) * this->force * k * clockDirection * 360.0f);
				gvec3 pos;
				particle->setPosition(this->position + this->_rotation * (position - this->position));
			}
		}

	}

}

