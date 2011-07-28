#include <math.h>

#include <gtypes/Matrix3.h>

#include "Particle.h"
#include "Swirl.h"

namespace aprilparticle
{
	gmat3 rotation;

	Swirl::Swirl()
	{
		this->position = gvec3(0.0f, 0.0f, 0.0f);
		this->up = gvec3(0.0f, 1.0f, 0.0f);
		this->range = 1.0f;
		this->force = 1.0f;
		this->clockwise = true;
	}

	Swirl::Swirl(gvec3 position, gvec3 up, float range, float force)
	{
		this->position = position;
		this->up = up;
		this->range = range;
		this->force = force;
		this->clockwise = true;
	}
		
	Swirl::~Swirl()
	{
	}
		
	void Swirl::update(Particle* particle, float k)
	{
		gvec3 direction = particle->position - this->position;
		float sqlen = direction.squaredLength();
		if (sqlen < this->range * this->range * 0.25f)
		{
			rotation.setRotation3D(this->up, (1.0f - sqrt(sqlen) / this->range) * this->force * k * (this->clockwise ? -360.0f : 360.0f));
			particle->position = this->position + rotation * direction;
		}
	}

}

