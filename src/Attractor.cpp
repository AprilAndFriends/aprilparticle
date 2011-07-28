#include "Attractor.h"
#include "Particle.h"

namespace aprilparticle
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
		gvec3 direction = this->position - particle->position;
		if (direction.squaredLength() > 0.02f)
		{
			float len = direction.length();
			particle->position += direction * (this->radius - len / this->radius) * (this->radius / (len * len + 1.0f)) * k / this->radius;
		}
	}

}

