/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <math.h>

#include <gtypes/Matrix3.h>

#include "AffectorEvolutor.h"
#include "aprilparticle.h"
#include "Particle.h"

namespace aprilparticle
{
	namespace Affectors
	{
		gmat3 _rotation;

		Evolutor::Evolutor(chstr name) : Affector(name)
		{
			this->position = gvec3(0.0f, 0.0f, 0.0f);
			this->axis = gvec3(0.0f, 1.0f, 0.0f);
			this->force = 1.0f;
			this->range = 1.0f;
			this->setClockwise(true);
		}

		Evolutor::Evolutor(gvec3 position, gvec3 axis, float force, float range, bool clockwise, chstr name) : Affector(name)
		{
			this->position = position;
			this->axis = axis;
			this->force = force;
			this->range = range;
			this->setClockwise(clockwise);
		}
		
		Evolutor::~Evolutor()
		{
		}
		
		bool Evolutor::getClockwise()
		{
			return (this->angle >= 0.0f);
		}

		void Evolutor::setClockwise(bool value)
		{
			this->angle = (value ? -360.0f : 360.0f);
		}

		hstr Evolutor::getProperty(chstr name, bool* property_exists)
		{
			if (property_exists != NULL)
			{
				*property_exists = true;
			}
			if (name == "position")		return gvec3_to_str(this->getPosition());
			if (name == "axis")			return gvec3_to_str(this->getAxis());
			if (name == "force")		return this->getForce();
			if (name == "range")		return this->getRange();
			if (name == "clockwise")	return this->getClockwise();
			return Affector::getProperty(name, property_exists);
		}

		bool Evolutor::setProperty(chstr name, chstr value)
		{
			if		(name == "position")	this->setPosition(str_to_gvec3(value));
			else if	(name == "axis")		this->setAxis(str_to_gvec3(value));
			else if	(name == "force")		this->setForce(value);
			else if	(name == "range")		this->setRange(value);
			else if	(name == "clockwise")	this->setClockwise(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void Evolutor::update(Particle* particle, float k)
		{
			this->_direction = particle->position - this->position;
			this->_squaredLength = this->_direction.squaredLength();
			if (this->_squaredLength < this->range * this->range * 0.25f)
			{
				_rotation.setRotation3D(this->axis, (1.0f - sqrt(this->_squaredLength) / this->range) * this->force * k * this->angle);
				particle->position = this->position + _rotation * this->_direction;
			}
		}

	}

}

