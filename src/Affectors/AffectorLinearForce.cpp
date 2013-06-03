/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "AffectorLinearForce.h"
#include "aprilparticleUtil.h"
#include "Particle.h"

namespace aprilparticle
{
	namespace Affectors
	{
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

		hstr LinearForce::getProperty(chstr name, bool* property_exists)
		{
			if (property_exists != NULL)
			{
				*property_exists = true;
			}
			if (name == "direction")	return gvec3_to_hstr(this->getDirection());
			return Affector::getProperty(name, property_exists);
		}

		bool LinearForce::setProperty(chstr name, chstr value)
		{
			if		(name == "direction")	this->setDirection(hstr_to_gvec3(value));
			else return Affector::setProperty(name, value);
			return true;
		}

		void LinearForce::update(Particle* particle, float k, gvec3& movement)
		{
			particle->direction += this->direction * (RAND_RANGE(Randomness) * k);
		}

	}

}

