#include "LinearDirectionalForce.h"
#include "Particle.h"

namespace aprilparticle
{
	LinearDirectionalForce::LinearDirectionalForce(gvec3 direction, float force)
	{
		this->direction = direction;
		this->force = force;
	}

	LinearDirectionalForce::~LinearDirectionalForce()
	{
	}
		
	void LinearDirectionalForce::update(Particle* particle, float k)
	{
		particle->speed += this->force * k * 0.001f;
		particle->position += this->direction * particle->speed * particle->direction.length();
	}

}

