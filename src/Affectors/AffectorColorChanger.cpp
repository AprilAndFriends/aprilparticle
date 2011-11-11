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
#include <hltypes/util.h>

#include "AffectorColorChanger.h"
#include "Particle.h"

namespace aprilparticle
{
	namespace Affectors
	{
		ColorChanger::ColorChanger(chstr name) : Affector(name)
		{
			this->startColor = APRIL_COLOR_WHITE;
			this->endColor = april::Color(APRIL_COLOR_WHITE, 0);
		}
		
		ColorChanger::ColorChanger(april::Color endColor, chstr name) : Affector(name)
		{
			this->startColor = APRIL_COLOR_WHITE;
			this->endColor = endColor;
		}
		
		ColorChanger::ColorChanger(april::Color startColor, april::Color endColor, chstr name) : Affector(name)
		{
			this->startColor = startColor;
			this->endColor = endColor;
		}

		ColorChanger::~ColorChanger()
		{
		}
	
		hstr ColorChanger::getProperty(chstr name, bool* property_exists)
		{
			if (property_exists != NULL)
			{
				*property_exists = true;
			}
			if (name == "start_color")	return this->getStartColor().hex();
			if (name == "end_color")	return this->getEndColor().hex();
			return Affector::getProperty(name, property_exists);
		}

		bool ColorChanger::setProperty(chstr name, chstr value)
		{
			if		(name == "start_color")	this->setStartColor(value);
			else if	(name == "end_color")	this->setEndColor(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void ColorChanger::update(Particle* particle, float k)
		{
			this->_ratio = particle->getLifeProgressRatio();
			particle->color = this->startColor * (1.0f - this->_ratio) + this->endColor * this->_ratio;
		}

	}

}

