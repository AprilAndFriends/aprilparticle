/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

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
		ResizerTimed::ResizerTimed(chstr name) : Affector(name)
		{
			TIMED_TEMPLATE_INIT(gvec2(1.0f, 1.0f), gvec2(0.0f, 0.0f));
		}
		
		ResizerTimed::ResizerTimed(hmap<float, gvec2> timings, chstr name) : Affector(name)
		{
			this->setTimings(timings);
		}

		ResizerTimed::~ResizerTimed()
		{
		}

		Affector* ResizerTimed::createInstance(chstr name)
		{
			return new ResizerTimed(name);
		}

		void ResizerTimed::setTimings(hmap<float, gvec2> value)
		{
			TIMED_TEMPLATE_SET_TIMINGS;
		}
		
		void ResizerTimed::setTimings(chstr value)
		{
			TIMED_TEMPLATE_SET_TIMINGS_STRING(gvec2, hstr_to_gvec2);
		}
		
		void ResizerTimed::addTiming(float time, gvec2 value)
		{
			TIMED_TEMPLATE_ADD_TIMING;
		}

		hstr ResizerTimed::getProperty(chstr name, bool* property_exists)
		{
			if (property_exists != NULL)
			{
				*property_exists = true;
			}
			if (name == "timings")
			{
				TIMED_TEMPLATE_GET_TIMINGS_PROPERTY(gvec2_to_hstr, ); // conversion using gvec2_to_hstr prefix
			}
			return Affector::getProperty(name, property_exists);
		}

		bool ResizerTimed::setProperty(chstr name, chstr value)
		{
			if (name == "timings")	this->setTimings(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void ResizerTimed::update(Particle* particle, float k, gvec3& movement)
		{
			TIMED_TEMPLATE_UPDATE(particle, size);
		}
		
	}

}

