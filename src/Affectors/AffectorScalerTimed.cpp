/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

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
		ScalerTimed::ScalerTimed(chstr name) : Affector(name)
		{
			TIMED_TEMPLATE_INIT(1.0f, 0.0f);
		}
		
		ScalerTimed::ScalerTimed(hmap<float, float> timings, chstr name) : Affector(name)
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

		void ScalerTimed::setTimings(hmap<float, float> value)
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

		hstr ScalerTimed::getProperty(chstr name, bool* property_exists)
		{
			if (property_exists != NULL)
			{
				*property_exists = true;
			}
			if (name == "timings")
			{
				TIMED_TEMPLATE_GET_TIMINGS_PROPERTY(hstr, ); // conversion using hstr prefix
			}
			return Affector::getProperty(name, property_exists);
		}

		bool ScalerTimed::setProperty(chstr name, chstr value)
		{
			if (name == "timings")	this->setTimings(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void ScalerTimed::update(Particle* particle, float k, gvec3& movement)
		{
			TIMED_TEMPLATE_UPDATE(particle, scale);
		}
		
	}

}

