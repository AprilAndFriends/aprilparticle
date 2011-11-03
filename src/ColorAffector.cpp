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

#include "ColorAffector.h"
#include "Particle.h"

namespace aprilparticle
{
	ColorAffector::ColorAffector()
	{
		this->startColor = APRIL_COLOR_WHITE;
		this->endColor = april::Color(APRIL_COLOR_WHITE, 0);
	}
		
	ColorAffector::ColorAffector(april::Color endColor)
	{
		this->startColor = APRIL_COLOR_WHITE;
		this->endColor = endColor;
	}
		
	ColorAffector::ColorAffector(april::Color startColor, april::Color endColor)
	{
		this->startColor = startColor;
		this->endColor = endColor;
	}

	ColorAffector::~ColorAffector()
	{
	}
	
	void ColorAffector::update(Particle* particle, float k)
	{
		this->_ratio = particle->getLifeProgressRatio();
		particle->color = this->startColor * this->_ratio + this->endColor * (1.0f - this->_ratio);
	}

}

