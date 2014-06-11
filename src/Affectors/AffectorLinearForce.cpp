/// @file
/// @version 2.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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

		hstr LinearForce::getProperty(chstr name)
		{
			if (name == "direction")	return gvec3_to_hstr(this->getDirection());
			return Affector::getProperty(name);
		}

		bool LinearForce::setProperty(chstr name, chstr value)
		{
			if		(name == "direction")	this->setDirection(hstr_to_gvec3(value));
			else return Affector::setProperty(name, value);
			return true;
		}

		void LinearForce::update(Particle* particle, float timeDelta, gvec3& movement)
		{
			particle->direction += this->direction * (RAND_RANGE(Randomness) * timeDelta);
		}

	}

}

