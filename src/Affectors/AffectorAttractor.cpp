/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "AffectorAttractor.h"
#include "Particle.h"

namespace aprilparticle
{
	namespace Affectors
	{
		Attractor::Attractor() : Affector()
		{
		}
		
		Attractor::Attractor(gvec3 position, float radius)
		{
			this->position = position;
			this->radius = radius;
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
			if (name == "x")		return this->getX();
			if (name == "y")		return this->getY();
			if (name == "z")		return this->getZ();
			if (name == "radius")	return this->getRadius();
			return Affector::getProperty(name, property_exists);
		}

		bool Attractor::setProperty(chstr name, chstr value)
		{
			if		(name == "x")		this->setX(value);
			else if	(name == "y")		this->setY(value);
			else if	(name == "z")		this->setZ(value);
			else if	(name == "radius")	this->setRadius(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void Attractor::update(Particle* particle, float k)
		{
			this->_direction = this->position - particle->position;
			if (this->_direction.squaredLength() > 0.02f)
			{
				this->_length = this->_direction.length();
				particle->position += this->_direction * (this->radius - this->_length / this->radius) * (this->radius / (this->_length * this->_length + 1.0f)) * k / this->radius;
			}
		}

	}

}

