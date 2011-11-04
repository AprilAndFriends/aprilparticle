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
#include "Particle.h"

namespace aprilparticle
{
	namespace Affectors
	{
		gmat3 rotation;

		Swirl::Swirl()
		{
			this->position = gvec3(0.0f, 0.0f, 0.0f);
			this->up = gvec3(0.0f, 1.0f, 0.0f);
			this->force = 1.0f;
			this->range = 1.0f;
			this->setClockwise(true);
		}

		Swirl::Swirl(gvec3 position, gvec3 up, float force, float range)
		{
			this->position = position;
			this->up = up;
			this->force = force;
			this->range = range;
			this->setClockwise(true);
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
			if (name == "x")			return this->getX();
			if (name == "y")			return this->getY();
			if (name == "z")			return this->getZ();
			if (name == "up_x")			return this->getUpX();
			if (name == "up_y")			return this->getUpY();
			if (name == "up_z")			return this->getUpZ();
			if (name == "force")		return this->getForce();
			if (name == "range")		return this->getRange();
			if (name == "clockwise")	return this->getClockwise();
			return Affector::getProperty(name, property_exists);
		}

		bool Swirl::setProperty(chstr name, chstr value)
		{
			if		(name == "x")			this->setX(value);
			else if	(name == "y")			this->setY(value);
			else if	(name == "z")			this->setZ(value);
			else if	(name == "up_x")		this->setUpX(value);
			else if	(name == "up_y")		this->setUpY(value);
			else if	(name == "up_z")		this->setUpZ(value);
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

