/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "Attractor.h"
#include "Particle.h"

namespace aprilparticle
{
	Attractor::Attractor()
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

