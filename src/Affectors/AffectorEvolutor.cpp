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
#include "System.h"

namespace aprilparticle
{
	namespace Affectors
	{
		gmat3 _rotation;

		Evolutor::Evolutor(chstr name) : Affector(name)
		{
			this->position = gvec3(0.0f, 0.0f, 0.0f);
			this->axis = gvec3(0.0f, 1.0f, 0.0f);
			this->radius = 1.0f;
			this->evolutionSpeed = 1.0f;
			this->setClockwise(true);
		}

		Evolutor::Evolutor(gvec3 position, gvec3 axis, float radius, float evolutionSpeed, bool clockwise, chstr name) : Affector(name)
		{
			this->position = position;
			this->axis = axis;
			this->radius = radius;
			this->evolutionSpeed = evolutionSpeed;
			this->setClockwise(clockwise);
		}
		
		Evolutor::~Evolutor()
		{
		}
		
		bool Evolutor::isClockwise()
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
			if (name == "position")			return gvec3_to_str(this->getPosition());
			if (name == "axis")				return gvec3_to_str(this->getAxis());
			if (name == "radius")			return this->getRadius();
			if (name == "evolution_speed")	return this->getEvolutionSpeed();
			if (name == "clockwise")		return this->isClockwise();
			return Affector::getProperty(name, property_exists);
		}

		bool Evolutor::setProperty(chstr name, chstr value)
		{
			if		(name == "position")		this->setPosition(str_to_gvec3(value));
			else if	(name == "axis")			this->setAxis(str_to_gvec3(value));
			else if	(name == "radius")			this->setRadius(value);
			else if	(name == "evolution_speed")	this->setEvolutionSpeed(value);
			else if	(name == "clockwise")		this->setClockwise(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void Evolutor::update(Particle* particle, float k)
		{
			this->_position = this->position + this->system->getPosition();
			this->_direction = particle->position - this->_position;
			this->_squaredLength = this->_direction.squaredLength();
			if (this->_squaredLength < this->radius * this->radius)
			{
				_rotation.setRotation3D(this->axis, (1.0f - sqrt(this->_squaredLength) / this->radius) * this->evolutionSpeed * k * this->angle);
				particle->position = this->_position + _rotation * this->_direction;
			}
		}

	}

}

