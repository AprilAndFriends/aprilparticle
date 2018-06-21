/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/aprilUtil.h>
#include <gtypes/Vector2.h>
#include <hltypes/hmap.h>
#include <hltypes/harray.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "AffectorResizerTimed.h"
#include "aprilparticle.h"
#include "aprilparticleUtil.h"
#include "Particle.h"
#include "TimedTemplate.h"

namespace aprilparticle
{
	namespace Affectors
	{
		hmap<hstr, PropertyDescription> ResizerTimed::_propertyDescriptions;

		ResizerTimed::ResizerTimed(chstr name) :
			Affector(name),
			size(0),
			_ratio(0.0f),
			_i(0)
		{
			this->type = "Resizer";
			TIMED_TEMPLATE_INIT(gvec2f(1.0f, 1.0f), gvec2f(0.0f, 0.0f));
		}
		
		ResizerTimed::ResizerTimed(const hmap<float, gvec2f>& timings, chstr name) :
			Affector(name),
			size(0),
			_ratio(0.0f),
			_i(0)
		{
			this->type = "Resizer";
			this->setTimings(timings);
		}

		ResizerTimed::ResizerTimed(const ResizerTimed& other) :
			Affector(other),
			size(0),
			_ratio(0.0f),
			_i(0)
		{
			this->times = other.times;
			this->values = other.values;
		}

		ResizerTimed::~ResizerTimed()
		{
		}

		Affector* ResizerTimed::createInstance(chstr name)
		{
			return new ResizerTimed(name);
		}

		void ResizerTimed::setTimings(const hmap<float, gvec2f>& value)
		{
			TIMED_TEMPLATE_SET_TIMINGS;
		}
		
		void ResizerTimed::setTimings(chstr value)
		{
			TIMED_TEMPLATE_SET_TIMINGS_STRING(gvec2f, april::hstrToGvec2f);
		}
		
		void ResizerTimed::addTiming(float time, cgvec2f value)
		{
			TIMED_TEMPLATE_ADD_TIMING;
		}

		TIMED_TEMPLATE_PROPERTY_DESCRIPTIONS(ResizerTimed, Affector, Gvec2f);

		hstr ResizerTimed::getProperty(chstr name)
		{
			if (name == "timings")
			{
				TIMED_TEMPLATE_GET_TIMINGS_PROPERTY(april::gvec2fToHstr, ); // conversion using april::gvec2fToHstr prefix
			}
			return Affector::getProperty(name);
		}

		bool ResizerTimed::setProperty(chstr name, chstr value)
		{
			if (name == "timings")	this->setTimings(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void ResizerTimed::update(Particle* particle, float timeDelta, gvec3f& movement)
		{
			TIMED_TEMPLATE_UPDATE(particle, size);
		}
		
	}

}

