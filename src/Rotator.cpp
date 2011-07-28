#include "Particle.h"
#include "Rotator.h"

namespace aprilparticle
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
		particle->angle += this->rotationSpeed * k;
	}

}

