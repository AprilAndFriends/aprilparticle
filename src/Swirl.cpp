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

#include "Particle.h"
#include "Swirl.h"

namespace aprilparticle
{
	gmat3 rotation;

	Swirl::Swirl()
	{
		this->position = gvec3(0.0f, 0.0f, 0.0f);
		this->up = gvec3(0.0f, 1.0f, 0.0f);
		this->range = 1.0f;
		this->force = 1.0f;
		this->setClockwise(true);
	}

	Swirl::Swirl(gvec3 position, gvec3 up, float range, float force)
	{
		this->position = position;
		this->up = up;
		this->range = range;
		this->force = force;
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

