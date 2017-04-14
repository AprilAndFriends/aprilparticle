/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <aprilparticle/Emitter.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "apriluiparticleUtil.h"

namespace apriluiparticle
{
	void resizeEmitter(cgvec2 size, aprilparticle::Emitter* emitter)
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

	void resizeEmitters(cgvec2 size, const harray<aprilparticle::Emitter*>& emitters)
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
			foreachc (aprilparticle::Emitter*, it, emitters)
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
			foreachc (aprilparticle::Emitter*, it, emitters)
			{
				dimensions = (*it)->getDimensions();
				dimensions.y = (firstDimensions.y > 0.0f ? dimensions.y * rescale.y : rescale.y);
				(*it)->setDimensions(dimensions);
			}
		}
	}

}
