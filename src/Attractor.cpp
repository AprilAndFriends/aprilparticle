#include "Attractor.h"
#include "Particle.h"

namespace april
{
	namespace Affectors
	{
		Attractor::Attractor()
		{
		}
		
		Attractor::Attractor(gvec3 position, float radius)
		{
			this->position = position;
			this->radius = radius;
		}

		Attractor::~Attractor()
		{
		}
		
		void Attractor::update(Particle* particle, float k)
		{
			gvec3 position = particle->getPosition();
			gvec3 mDirection(this->position - position);
			if (mDirection.squaredLength() > 0.02f)
			{
				float len = (position - this->position).length();
				float lenSquared = (position - this->position).squaredLength();
				position += mDirection * (this->radius - (len) / this->radius) * (this->radius / (lenSquared + 1.0f)) * k / this->radius;
				particle->setPosition(position);
			}
		}

	}

}

