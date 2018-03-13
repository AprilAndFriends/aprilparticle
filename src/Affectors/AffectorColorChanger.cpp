/// @file
/// @version 2.3
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
		hmap<hstr, PropertyDescription> ColorChanger::_propertyDescriptions;

		ColorChanger::ColorChanger(chstr name) : Affector(name)
		{
			this->startColor = april::Color::White;
			this->endColor = april::Color::Blank;
		}
		
		ColorChanger::ColorChanger(const april::Color& endColor, chstr name) : Affector(name)
		{
			this->startColor = april::Color::White;
			this->endColor = endColor;
		}
		
		ColorChanger::ColorChanger(const april::Color& startColor, const april::Color& endColor, chstr name) : Affector(name)
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

		hmap<hstr, PropertyDescription>& ColorChanger::getPropertyDescriptions() const
		{
			if (ColorChanger::_propertyDescriptions.size() == 0)
			{
				ColorChanger::_propertyDescriptions = Affector::getPropertyDescriptions();
				ColorChanger::_propertyDescriptions["start_color"] = PropertyDescription("start_color", PropertyDescription::Type::Color);
				ColorChanger::_propertyDescriptions["end_color"] = PropertyDescription("end_color", PropertyDescription::Type::Color);
			}
			return ColorChanger::_propertyDescriptions;
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

