/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "AffectorLinearDirectionalForce.h"
#include "Particle.h"

namespace aprilparticle
{
	namespace Affectors
	{
		LinearDirectionalForce::LinearDirectionalForce(gvec3 direction, float force)
		{
			this->direction = direction;
			this->force = force;
		}

		LinearDirectionalForce::~LinearDirectionalForce()
		{
		}
		
		hstr LinearDirectionalForce::getProperty(chstr name, bool* property_exists)
		{
			if (property_exists != NULL)
			{
				*property_exists = true;
			}
			if (name == "dir_x")	return this->getDirectionX();
			if (name == "dir_y")	return this->getDirectionY();
			if (name == "dir_z")	return this->getDirectionZ();
			if (name == "force")	return this->getForce();
			return Affector::getProperty(name, property_exists);
		}

		bool LinearDirectionalForce::setProperty(chstr name, chstr value)
		{
			if		(name == "dir_x")	this->setDirectionX(value);
			else if	(name == "dir_y")	this->setDirectionY(value);
			else if	(name == "dir_z")	this->setDirectionZ(value);
			else if	(name == "force")	this->setForce(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void LinearDirectionalForce::update(Particle* particle, float k)
		{
			particle->speed += this->force * k;
			particle->position += this->direction * particle->speed * particle->direction.length();
		}

	}

}

