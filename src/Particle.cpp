/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Color.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/util.h>

#include "Particle.h"

namespace aprilparticle
{
	Particle::Particle(gvec3 position) : Space3DObject(position)
	{
		this->timer = 0.0f;
		this->life = 1.0f;
		this->size.set(1.0f, 1.0f);
		this->scale = 1.0f;
		this->speed = 0.0f;
		this->angle = 0.0f;
		this->color = APRIL_COLOR_WHITE;
	}
	
	Particle::~Particle()
	{
	}

	float Particle::getLifeProgressRatio()
	{
		return (this->life > 0.0f ? (hclamp(this->timer / this->life, 0.0f, 1.0f)) : 1.0f);
	}

	bool Particle::isDead()
	{
		return (this->timer >= this->life);
	}

}


