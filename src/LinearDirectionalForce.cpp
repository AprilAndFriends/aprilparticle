/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "LinearDirectionalForce.h"
#include "Particle.h"

namespace aprilparticle
{
	LinearDirectionalForce::LinearDirectionalForce(gvec3 direction, float force)
	{
		this->direction = direction;
		this->force = force;
	}

	LinearDirectionalForce::~LinearDirectionalForce()
	{
	}
		
	void LinearDirectionalForce::update(Particle* particle, float k)
	{
		particle->speed += this->force * k;
		particle->position += this->direction * particle->speed * particle->direction.length();
	}

}

