/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "AffectorAttractor.h"
#include "aprilparticle.h"
#include "Particle.h"
#include "System.h"

namespace aprilparticle
{
	namespace Affectors
	{
		Attractor::Attractor(chstr name) : Affector(name)
		{
			this->position.set(0.0f, 0.0f, 0.0f);
			this->radius = 10.0f;
			this->force = 1.0f;
		}
		
		Attractor::Attractor(gvec3 position, float radius, float force, chstr name) : Affector(name)
		{
			this->position = position;
			this->radius = radius;
			this->force = force;
		}

		Attractor::~Attractor()
		{
		}
		
		hstr Attractor::getProperty(chstr name, bool* property_exists)
		{
			if (property_exists != NULL)
			{
				*property_exists = true;
			}
			if (name == "position")		return gvec3_to_str(this->getPosition());
			if (name == "radius")		return this->getRadius();
			if (name == "force")		return this->getForce();
			return Affector::getProperty(name, property_exists);
		}

		bool Attractor::setProperty(chstr name, chstr value)
		{
			if		(name == "position")	this->setPosition(str_to_gvec3(value));
			else if	(name == "radius")		this->setRadius(value);
			else if	(name == "force")		this->setForce(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void Attractor::update(Particle* particle, float k, gvec3& movement)
		{
			this->_direction = this->position + this->system->getPosition() - particle->position;
			this->_squaredLength = this->_direction.squaredLength();
			if (is_inside(this->_squaredLength, 0.02f, this->radius * this->radius))
			{
				this->_factor = (this->radius - sqrt(this->_squaredLength)) / this->radius;
				movement += this->_direction.normalized() * (this->force * this->_factor * this->_factor * k);
			}
		}

	}

}

