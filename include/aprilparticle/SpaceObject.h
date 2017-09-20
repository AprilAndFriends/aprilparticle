/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
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

		harray<PropertyDescription> getPropertyDescriptions() const;

		HL_DEFINE_GETSET(gvec3, position, Position);
		inline void setPosition(float x, float y, float z) { this->position.set(x, y, z); }

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

	protected:
		gvec3 position;

	private:
		static harray<PropertyDescription> _propertyDescriptions;

	};
}

#endif
