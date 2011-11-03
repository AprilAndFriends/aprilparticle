/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Color.h>
#include <hltypes/util.h>

#include "Particle.h"

namespace aprilparticle
{
	Particle::Particle()
	{
		this->position = gvec3(0.0f, 0.0f, 0.0f);
		this->direction = gvec3(0.0f, 1.0f, 0.0f);
		this->life = 1.0f;
		this->totalLife = 1.0f;
		this->size = 1.0f;
		this->speed = 0.0f;
		this->angle = 0.0f;
		this->color = APRIL_COLOR_WHITE;
	}
	
	Particle::Particle(gvec3 position, gvec3 direction, float life, float size)
	{
		this->position = position;
		this->direction = direction;
		this->life = life;
		this->totalLife = life;
		this->size = size;
		this->speed = 0.0f;
		this->angle = 0.0f;
		this->color = APRIL_COLOR_WHITE;
	}
	
	Particle::~Particle()
	{
	}

	float Particle::getLifeProgressRatio()
	{
		return (1.0f - (this->totalLife > 0.0f ? hclamp(this->life / this->totalLife, 0.0f, 1.0f) : 1.0f));
	}

	bool Particle::isDead()
	{
		return (this->life <= 0.0f);
	}

}


