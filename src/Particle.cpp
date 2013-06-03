/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Color.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>

#include "Particle.h"

namespace aprilparticle
{
	Particle::Particle()
	{
		this->timer = 0.0f;
		this->life = 1.0f;
		this->position.set(0.0f, 0.0f, 0.0f);
		this->direction.set(0.0f, 0.0f, 0.0f);
		this->size.set(1.0f, 1.0f);
		this->scale = 1.0f;
		this->angle = 0.0f;
		this->color = april::Color::White;
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


