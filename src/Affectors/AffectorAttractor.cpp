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
		}
		
		Attractor::Attractor(gvec3 position, float force, chstr name) : Affector(name)
		{
			this->position = position;
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
			if (name == "force")		return this->getForce();
			return Affector::getProperty(name, property_exists);
		}

		bool Attractor::setProperty(chstr name, chstr value)
		{
			if		(name == "position")	this->setPosition(str_to_gvec3(value));
			else if	(name == "force")		this->setForce(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void Attractor::update(Particle* particle, float k)
		{
			this->_direction = this->position + this->system->getPosition() - particle->position;
			this->_squaredLength = this->_direction.squaredLength();
			if (this->_squaredLength > 0.02f)
			{
				particle->position += this->_direction * (((this->force * this->force - sqrt(this->_squaredLength)) / (this->force * (this->_squaredLength + 1.0f))) * k);
			}
		}

	}

}

