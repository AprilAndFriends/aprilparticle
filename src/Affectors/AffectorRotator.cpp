/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "AffectorRotator.h"
#include "aprilparticleUtil.h"
#include "Particle.h"

namespace aprilparticle
{
	namespace Affectors
	{
		hmap<hstr, PropertyDescription> Rotator::_propertyDescriptions;

		Rotator::Rotator(chstr name) :
			Affector(name)
		{
			this->type = "Rotator";
			this->rotationSpeed = 1.0f;
		}

		Rotator::Rotator(float rotationSpeed, chstr name) :
			Affector(name)
		{
			this->type = "Rotator";
			this->rotationSpeed = rotationSpeed;
		}

		Rotator::Rotator(const Rotator& other) :
			Affector(other)
		{
			this->rotationSpeed = other.rotationSpeed;
		}

		Affector* Rotator::createInstance(chstr name)
		{
			return new Rotator(name);
		}

		hmap<hstr, PropertyDescription>& Rotator::getPropertyDescriptions() const
		{
			if (Rotator::_propertyDescriptions.size() == 0)
			{
				Rotator::_propertyDescriptions = Affector::getPropertyDescriptions();
				Rotator::_propertyDescriptions["rotation_speed"] = PropertyDescription("rotation_speed", PropertyDescription::Type::Float, 1.0f);
			}
			return Rotator::_propertyDescriptions;
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

		void Rotator::update(Particle* particle, float timeDelta, gvec3f& movement)
		{
			particle->angle += RAND_RANGE(Randomness) * this->rotationSpeed * timeDelta;
		}

	}

}

