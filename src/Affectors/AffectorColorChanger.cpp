/// @file
/// @version 2.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/Color.h>
#include <hltypes/hltypesUtil.h>

#include "AffectorColorChanger.h"
#include "Particle.h"

namespace aprilparticle
{
	namespace Affectors
	{
		ColorChanger::ColorChanger(chstr name) : Affector(name)
		{
			this->startColor = april::Color::White;
			this->endColor = april::Color::Blank;
		}
		
		ColorChanger::ColorChanger(april::Color endColor, chstr name) : Affector(name)
		{
			this->startColor = april::Color::White;
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
	
		Affector* ColorChanger::createInstance(chstr name)
		{
			return new ColorChanger(name);
		}

		hstr ColorChanger::getProperty(chstr name)
		{
			if (name == "start_color")	return this->getStartColor().hex();
			if (name == "end_color")	return this->getEndColor().hex();
			return Affector::getProperty(name);
		}

		bool ColorChanger::setProperty(chstr name, chstr value)
		{
			if		(name == "start_color")	this->setStartColor(value);
			else if	(name == "end_color")	this->setEndColor(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void ColorChanger::update(Particle* particle, float timeDelta, gvec3& movement)
		{
			this->_ratio = particle->getLifeProgressRatio();
			particle->color = this->startColor * (1.0f - this->_ratio) + this->endColor * this->_ratio;
		}

	}

}

