/// @file
/// @version 2.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "AffectorRotator.h"
#include "aprilparticleUtil.h"
#include "Particle.h"

namespace aprilparticle
{
	namespace Affectors
	{
		Rotator::Rotator(chstr name) : Affector(name)
		{
			this->rotationSpeed = 1.0f;
		}

		Rotator::Rotator(float rotationSpeed, chstr name) : Affector(name)
		{
			this->rotationSpeed = rotationSpeed;
		}

		Rotator::~Rotator()
		{
		}
		
		Affector* Rotator::createInstance(chstr name)
		{
			return new Rotator(name);
		}

		hstr Rotator::getProperty(chstr name)
		{
			if (name == "rotation_speed")	return this->getRotationSpeed();
			return Affector::getProperty(name);
		}

		bool Rotator::setProperty(chstr name, chstr value)
		{
			if (name == "rotation_speed")	this->setRotationSpeed(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void Rotator::update(Particle* particle, float timeDelta, gvec3& movement)
		{
			particle->angle += RAND_RANGE(Randomness) * this->rotationSpeed * timeDelta;
		}

	}

}

