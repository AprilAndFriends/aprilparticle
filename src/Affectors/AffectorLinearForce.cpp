/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/aprilUtil.h>

#include "AffectorLinearForce.h"
#include "aprilparticleUtil.h"
#include "Particle.h"

namespace aprilparticle
{
	namespace Affectors
	{
		harray<PropertyDescription> LinearForce::_propertyDescriptions;

		LinearForce::LinearForce(chstr name) : Affector(name)
		{
			this->direction.set(0.0f, -1.0f, 0.0f);
		}

		LinearForce::LinearForce(gvec3 direction, chstr name) : Affector(name)
		{
			this->direction = direction;
		}

		LinearForce::~LinearForce()
		{
		}
		
		Affector* LinearForce::createInstance(chstr name)
		{
			return new LinearForce(name);
		}

		harray<PropertyDescription> LinearForce::getPropertyDescriptions() const
		{
			if (LinearForce::_propertyDescriptions.size() == 0)
			{
				LinearForce::_propertyDescriptions += PropertyDescription("direction", PropertyDescription::GVEC3);
			}
			return (Affector::getPropertyDescriptions() + LinearForce::_propertyDescriptions);
		}

		hstr LinearForce::getProperty(chstr name)
		{
			if (name == "direction")	return april::gvec3ToHstr(this->getDirection());
			return Affector::getProperty(name);
		}

		bool LinearForce::setProperty(chstr name, chstr value)
		{
			if		(name == "direction")	this->setDirection(april::hstrToGvec3(value));
			else return Affector::setProperty(name, value);
			return true;
		}

		void LinearForce::update(Particle* particle, float timeDelta, gvec3& movement)
		{
			particle->direction += this->direction * (RAND_RANGE(Randomness) * timeDelta);
		}

	}

}

