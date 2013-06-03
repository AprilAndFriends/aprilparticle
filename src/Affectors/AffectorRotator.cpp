/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

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

		hstr Rotator::getProperty(chstr name, bool* property_exists)
		{
			if (property_exists != NULL)
			{
				*property_exists = true;
			}
			if (name == "rotation_speed")	return this->getRotationSpeed();
			return Affector::getProperty(name, property_exists);
		}

		bool Rotator::setProperty(chstr name, chstr value)
		{
			if (name == "rotation_speed")	this->setRotationSpeed(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void Rotator::update(Particle* particle, float k, gvec3& movement)
		{
			particle->angle += RAND_RANGE(Randomness) * this->rotationSpeed * k;
		}

	}

}

