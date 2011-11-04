/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "Particle.h"
#include "AffectorRotator.h"

namespace aprilparticle
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
		
		hstr Rotator::getProperty(chstr name, bool* property_exists)
		{
			if (property_exists != NULL)
			{
				*property_exists = true;
			}
			if (name == "rotation_speed")	return this->getRotationSpeed();
			return Rotator::getProperty(name, property_exists);
		}

		bool Rotator::setProperty(chstr name, chstr value)
		{
			if (name == "rotation_speed")	this->setRotationSpeed(value);
			else return Rotator::setProperty(name, value);
			return true;
		}

		void Rotator::update(Particle* particle, float k)
		{
			particle->angle += this->rotationSpeed * k;
		}

	}

}

