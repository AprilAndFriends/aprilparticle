/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <math.h>

#include <gtypes/Matrix3.h>

#include "AffectorSwirl.h"
#include "aprilparticle.h"
#include "Particle.h"

namespace aprilparticle
{
	namespace Affectors
	{
		gmat3 rotation;

		Swirl::Swirl(chstr name) : Affector(name)
		{
			this->position = gvec3(0.0f, 0.0f, 0.0f);
			this->up = gvec3(0.0f, 1.0f, 0.0f);
			this->force = 1.0f;
			this->range = 1.0f;
			this->setClockwise(true);
		}

		Swirl::Swirl(gvec3 position, gvec3 up, float force, float range, bool clockwise, chstr name) : Affector(name)
		{
			this->position = position;
			this->up = up;
			this->force = force;
			this->range = range;
			this->setClockwise(clockwise);
		}
		
		Swirl::~Swirl()
		{
		}
		
		bool Swirl::getClockwise()
		{
			return (this->angle >= 0.0f);
		}

		void Swirl::setClockwise(bool value)
		{
			this->angle = (value ? -360.0f : 360.0f);
		}

		hstr Swirl::getProperty(chstr name, bool* property_exists)
		{
			if (property_exists != NULL)
			{
				*property_exists = true;
			}
			if (name == "position")		return gvec3_to_str(this->getPosition());
			if (name == "up")			return gvec3_to_str(this->getUp());
			if (name == "force")		return this->getForce();
			if (name == "range")		return this->getRange();
			if (name == "clockwise")	return this->getClockwise();
			return Affector::getProperty(name, property_exists);
		}

		bool Swirl::setProperty(chstr name, chstr value)
		{
			if		(name == "position")	this->setPosition(str_to_gvec3(value));
			else if	(name == "up")			this->setUp(str_to_gvec3(value));
			else if	(name == "force")		this->setForce(value);
			else if	(name == "range")		this->setRange(value);
			else if	(name == "clockwise")	this->setClockwise(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void Swirl::update(Particle* particle, float k)
		{
			this->_direction = particle->position - this->position;
			this->_squaredLength = this->_direction.squaredLength();
			if (this->_squaredLength < this->range * this->range * 0.25f)
			{
				rotation.setRotation3D(this->up, (1.0f - sqrt(this->_squaredLength) / this->range) * this->force * k * this->angle);
				particle->position = this->position + rotation * this->_direction;
			}
		}

	}

}

