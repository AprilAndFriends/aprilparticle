/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hmap.h>
#include <hltypes/harray.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "AffectorScalerTimed.h"
#include "aprilparticle.h"
#include "Particle.h"
#include "TimedTemplate.h"

namespace aprilparticle
{
	namespace Affectors
	{
		hmap<hstr, PropertyDescription> ScalerTimed::_propertyDescriptions;

		ScalerTimed::ScalerTimed(chstr name) : Affector(name)
		{
			TIMED_TEMPLATE_INIT(1.0f, 0.0f);
		}
		
		ScalerTimed::ScalerTimed(const hmap<float, float>& timings, chstr name) : Affector(name)
		{
			this->setTimings(timings);
		}

		ScalerTimed::~ScalerTimed()
		{
		}

		Affector* ScalerTimed::createInstance(chstr name)
		{
			return new ScalerTimed(name);
		}

		void ScalerTimed::setTimings(const hmap<float, float>& value)
		{
			TIMED_TEMPLATE_SET_TIMINGS;
		}
		
		void ScalerTimed::setTimings(chstr value)
		{
			TIMED_TEMPLATE_SET_TIMINGS_STRING(float, ); // no special constructor needed for "float"
		}
		
		void ScalerTimed::addTiming(float time, float value)
		{
			TIMED_TEMPLATE_ADD_TIMING;
		}

		TIMED_TEMPLATE_PROPERTY_DESCRIPTIONS(ScalerTimed, Affector, Float);

		hstr ScalerTimed::getProperty(chstr name)
		{
			if (name == "timings")
			{
				TIMED_TEMPLATE_GET_TIMINGS_PROPERTY(hstr, ); // conversion using hstr prefix
			}
			return Affector::getProperty(name);
		}

		bool ScalerTimed::setProperty(chstr name, chstr value)
		{
			if (name == "timings")	this->setTimings(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void ScalerTimed::update(Particle* particle, float timeDelta, gvec3f& movement)
		{
			TIMED_TEMPLATE_UPDATE(particle, scale);
		}
		
	}

}

