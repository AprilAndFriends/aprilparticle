#include "LinearDirectionalForce.h"
#include "Particle.h"

namespace april
{
	namespace Affectors
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
			float speed = particle->getSpeed() + this->force * k * 0.001f;
			particle->setSpeed(speed);
			particle->setPosition(particle->getPosition() + this->direction * speed * particle->getDirection().length());
		}
		
	}

}

