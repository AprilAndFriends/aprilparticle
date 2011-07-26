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
		
		void Rotator::update(Particle* particle, float k)
		{
			particle->setAngle(particle->getAngle() + this->rotationSpeed * k);
		}

	}

}

