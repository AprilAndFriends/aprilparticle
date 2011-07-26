#include "Attractor.h"
#include "Particle.h"

namespace april
{
	namespace Affectors
	{
		Attractor::Attractor()
		{
		}
		
		Attractor::Attractor(gvec3 origin, float radius)
		{
			this->origin = origin;
			this->radius = radius;
		}

		Attractor::~Attractor()
		{
		}
		
		void Attractor::draw()
		{
		}
		
		void Attractor::update(Particle* particle, double t)
		{
			gvec3 position = particle->getPosition();
			gvec3 mDirection(this->origin - position);
			if (mDirection.squaredLength() > 0.02f)
			{
				float len = (position - this->origin).length();
				float lenSquared = (position - this->origin).squaredLength();
				position += mDirection * (this->radius - (len) / this->radius) * (this->radius / (lenSquared + 1.0f)) * (float)t / this->radius;
				particle->setPosition(position);
			}
			//else
			//	particle->mLife = -1.0;
		}

	}

}

