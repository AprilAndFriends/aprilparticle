/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
		hmap<hstr, PropertyDescription> ColorChangerTimed::_propertyDescriptions;

		ColorChangerTimed::ColorChangerTimed(chstr name) : Affector(name), size(0), _ratio(0.0f), _i(0)
		{
			this->type = "ColorChanger";
			TIMED_TEMPLATE_INIT(april::Color::White, april::Color::Blank);
		}
		
		ColorChangerTimed::ColorChangerTimed(const hmap<float, april::Color>& timings, chstr name) : Affector(name), size(0), _ratio(0.0f), _i(0)
		{
			this->type = "ColorChanger";
			this->setTimings(timings);
		}

		ColorChangerTimed::ColorChangerTimed(const ColorChangerTimed& other) : Affector(other), size(0), _ratio(0.0f), _i(0)
		{
			this->times = other.times;
			this->values = other.values;
			this->size = other.size;
		}

		ColorChangerTimed::~ColorChangerTimed()
		{
		}

		Affector* ColorChangerTimed::createInstance(chstr name)
		{
			return new ColorChangerTimed(name);
		}

		void ColorChangerTimed::setTimings(const hmap<float, april::Color>& value)
		{
			TIMED_TEMPLATE_SET_TIMINGS;
		}
		
		void ColorChangerTimed::setTimings(chstr value)
		{
			TIMED_TEMPLATE_SET_TIMINGS_STRING(april::Color, april::Color);
		}
		
		void ColorChangerTimed::addTiming(float time, const april::Color& value)
		{
			TIMED_TEMPLATE_ADD_TIMING;
		}

		TIMED_TEMPLATE_PROPERTY_DESCRIPTIONS(ColorChangerTimed, Affector, Color);

		hstr ColorChangerTimed::getProperty(chstr name)
		{
			if (name == "timings")
			{
				TIMED_TEMPLATE_GET_TIMINGS_PROPERTY(, .hex()); // conversion using .hex() suffix
			}
			return Affector::getProperty(name);
		}

		bool ColorChangerTimed::setProperty(chstr name, chstr value)
		{
			if (name == "timings")	this->setTimings(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void ColorChangerTimed::update(Particle* particle, float timeDelta, gvec3f& movement)
		{
			TIMED_TEMPLATE_UPDATE(particle, color);
		}
		
	}
}

