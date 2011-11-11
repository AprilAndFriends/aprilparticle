/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "AffectorLinearForce.h"
#include "aprilparticle.h"
#include "Particle.h"

namespace aprilparticle
{
	namespace Affectors
	{
		LinearForce::LinearForce(chstr name) : Affector(name)
		{
			this->direction.set(0.0f, -1.0f, 0.0f);
			this->force = 1.0f;
		}

		LinearForce::LinearForce(gvec3 direction, float force, chstr name) : Affector(name)
		{
			this->direction = direction;
			this->force = force;
		}

		LinearForce::~LinearForce()
		{
		}
		
		hstr LinearForce::getProperty(chstr name, bool* property_exists)
		{
			if (property_exists != NULL)
			{
				*property_exists = true;
			}
			if (name == "direction")	return gvec3_to_str(this->getDirection());
			if (name == "force")		return this->getForce();
			return Affector::getProperty(name, property_exists);
		}

		bool LinearForce::setProperty(chstr name, chstr value)
		{
			if		(name == "direction")	this->setDirection(str_to_gvec3(value));
			else if	(name == "force")		this->setForce(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void LinearForce::update(Particle* particle, float k)
		{
			if (this->force != 0.0f)
			{
				particle->speed += this->force * k * 0.001f;
			}
			particle->position += this->direction * (particle->speed * particle->direction.length());
		}

	}

}

