/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Color.h>
#include <hltypes/hmap.h>
#include <hltypes/harray.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "AffectorColorChangerTimed.h"
#include "aprilparticle.h"
#include "Particle.h"
#include "TimedTemplate.h"

namespace aprilparticle
{
	namespace Affectors
	{
		ColorChangerTimed::ColorChangerTimed(chstr name) : Affector(name)
		{
			TIMED_TEMPLATE_INIT(april::Color::White, april::Color::Blank);
		}
		
		ColorChangerTimed::ColorChangerTimed(hmap<float, april::Color> timings, chstr name) : Affector(name)
		{
			this->setTimings(timings);
		}

		ColorChangerTimed::~ColorChangerTimed()
		{
		}

		Affector* ColorChangerTimed::createInstance(chstr name)
		{
			return new ColorChangerTimed(name);
		}

		void ColorChangerTimed::setTimings(hmap<float, april::Color> value)
		{
			TIMED_TEMPLATE_SET_TIMINGS;
		}
		
		void ColorChangerTimed::setTimings(chstr value)
		{
			TIMED_TEMPLATE_SET_TIMINGS_STRING(april::Color, april::Color);
		}
		
		void ColorChangerTimed::addTiming(float time, april::Color value)
		{
			TIMED_TEMPLATE_ADD_TIMING;
		}

		hstr ColorChangerTimed::getProperty(chstr name, bool* property_exists)
		{
			if (property_exists != NULL)
			{
				*property_exists = true;
			}
			if (name == "timings")
			{
				TIMED_TEMPLATE_GET_TIMINGS_PROPERTY(, .hex()); // conversion using .hex() suffix
			}
			return Affector::getProperty(name, property_exists);
		}

		bool ColorChangerTimed::setProperty(chstr name, chstr value)
		{
			if (name == "timings")	this->setTimings(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void ColorChangerTimed::update(Particle* particle, float k, gvec3& movement)
		{
			TIMED_TEMPLATE_UPDATE(particle, color);
		}
		
	}
}

