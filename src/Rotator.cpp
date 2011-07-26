#include "Particle.h"
#include "Rotator.h"

namespace april
{
	namespace Affectors
	{
		
		Rotator::Rotator(float rotationSpeed)
		{
			this->rotationSpeed = rotationSpeed;
		}

		Rotator::~Rotator()
		{
		}
		
		void Rotator::draw()
		{
		}
		
		void Rotator::update(Particle* particle, double t)
		{
			particle->setAngle(particle->getAngle() + this->rotationSpeed * (float)t);
		}

	}

}

