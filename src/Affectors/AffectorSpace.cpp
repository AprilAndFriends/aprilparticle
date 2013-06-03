/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hstring.h>

#include "AffectorSpace.h"
#include "aprilparticleUtil.h"

namespace aprilparticle
{
	namespace Affectors
	{
		Space::Space(chstr name) : Affector(name)
		{
			this->position.set(0.0f, 0.0f, 0.0f);
			this->radius = 10.0f;
		}
		
		Space::Space(gvec3 position, float radius, chstr name) : Affector(name)
		{
			this->position = position;
			this->radius = radius;
		}

		Space::~Space()
		{
		}
		
		hstr Space::getProperty(chstr name, bool* property_exists)
		{
			if (property_exists != NULL)
			{
				*property_exists = true;
			}
			if (name == "position")	return gvec3_to_hstr(this->getPosition());
			if (name == "radius")	return this->getRadius();
			return Affector::getProperty(name, property_exists);
		}

		bool Space::setProperty(chstr name, chstr value)
		{
			if		(name == "position")	this->setPosition(hstr_to_gvec3(value));
			else if	(name == "radius")		this->setRadius(value);
			else return Affector::setProperty(name, value);
			return true;
		}

	}

}

