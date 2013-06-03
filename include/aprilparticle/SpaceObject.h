/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents an object in space.

#ifndef APRILPARTICLE_SPACE_OBJECT_H
#define APRILPARTICLE_SPACE_OBJECT_H

#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "ActiveObject.h"
#include "aprilparticleExport.h"

namespace aprilparticle
{
	class aprilparticleExport SpaceObject : public ActiveObject
	{
	public:
		SpaceObject(chstr name = "");
		SpaceObject(const SpaceObject& other);
		~SpaceObject();

		HL_DEFINE_GETSET(gvec3, position, Position);
		void setPosition(float x, float y, float z) { this->position.set(x, y, z); }

		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);

	protected:
		gvec3 position;

	};
}

#endif
