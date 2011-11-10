/// @file
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents an object in 3-dimensional space.

#ifndef APRILPARTICLE_SPACE_3D_OBJECT_H
#define APRILPARTICLE_SPACE_3D_OBJECT_H

#include <gtypes/Vector3.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "aprilparticleExport.h"

namespace aprilparticle
{
	class aprilparticleExport Space3DObject
	{
	public:
		Space3DObject(gvec3 position = gvec3(0.0f, 0.0f, 0.0f), gvec3 direction = gvec3(0.0f, 1.0f, 0.0f));
		virtual ~Space3DObject();

		HL_DEFINE_GETSET(gvec3, position, Position);
		HL_DEFINE_GETSET(gvec3, direction, Direction);
		void setPosition(float x, float y, float z) { this->position.set(x, y, z); }
		void setDirection(float x, float y, float z) { this->direction.set(x, y, z); }

		virtual hstr getProperty(chstr name, bool* property_exists = NULL);
		virtual bool setProperty(chstr name, chstr value);

	protected:
		gvec3 position;
		gvec3 direction;

	};
}

#endif
