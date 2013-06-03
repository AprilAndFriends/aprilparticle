/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <aprilparticle/Emitter.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "apriluiparticleUtil.h"

namespace apriluiparticle
{
	hstr generateName(chstr prefix)
	{
		static hmap<hstr, int> counters;
		int count = counters[prefix] + 1;
		counters[prefix] = count;
		return prefix + hstr(count);
	}
	
	void resizeEmitter(gvec2 size, aprilparticle::Emitter* emitter)
	{
		if (emitter == NULL)
		{
			return;
		}
		gvec3 dimensions = emitter->getDimensions();
		if (size.x > 0.0f)
		{
			dimensions.x = size.x;
			emitter->setDimensions(dimensions);
		}
		if (size.y > 0.0f)
		{
			dimensions.y = size.y;
			emitter->setDimensions(dimensions);
		}
	}

	void resizeEmitters(gvec2 size, harray<aprilparticle::Emitter*> emitters)
	{
		if (emitters.size() == 0)
		{
			return;
		}
		gvec3 dimensions;
		gvec3 firstDimensions = emitters.first()->getDimensions();
		gvec2 rescale = size;
		if (size.x > 0.0f)
		{
			if (firstDimensions.x > 0.0f)
			{
				rescale.x = size.x / firstDimensions.x;
			}
			foreach (aprilparticle::Emitter*, it, emitters)
			{
				dimensions = (*it)->getDimensions();
				dimensions.x = (firstDimensions.x > 0.0f ? dimensions.x * rescale.x : rescale.x);
				(*it)->setDimensions(dimensions);
			}
		}
		if (size.y > 0.0f)
		{
			if (firstDimensions.y > 0.0f)
			{
				rescale.y = size.y / firstDimensions.y;
			}
			foreach (aprilparticle::Emitter*, it, emitters)
			{
				dimensions = (*it)->getDimensions();
				dimensions.y = (firstDimensions.y > 0.0f ? dimensions.y * rescale.y : rescale.y);
				(*it)->setDimensions(dimensions);
			}
		}
	}

}
