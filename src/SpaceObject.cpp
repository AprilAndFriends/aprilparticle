/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "SpaceObject.h"
#include "Affector.h"
#include "aprilparticle.h"
#include "aprilparticleUtil.h"

namespace aprilparticle
{
	SpaceObject::SpaceObject(chstr name) : ActiveObject(name == "" ? generateName("SpaceObject") : name)
	{
		this->position.set(0.0f, 0.0f, 0.0f);
	}
	
	SpaceObject::SpaceObject(const SpaceObject& other) : ActiveObject(other)
	{
		this->position = other.position;
	}
	
	SpaceObject::~SpaceObject()
	{
	}

	hstr SpaceObject::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "position")	return gvec3_to_hstr(this->getPosition());
		return ActiveObject::getProperty(name, property_exists);
	}

	bool SpaceObject::setProperty(chstr name, chstr value)
	{
		if	(name == "position")	this->setPosition(hstr_to_gvec3(value));
		else return ActiveObject::setProperty(name, value);
		return true;
	}

}


