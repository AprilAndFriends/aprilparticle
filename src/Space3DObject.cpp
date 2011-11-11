/// @file
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "aprilparticle.h"
#include "Space3DObject.h"

namespace aprilparticle
{
	Space3DObject::Space3DObject(gvec3 position, gvec3 direction)
	{
		this->position = position;
		this->direction = direction;
	}
	
	Space3DObject::~Space3DObject()
	{
	}

	hstr Space3DObject::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "position")		return gvec3_to_str(this->getPosition());
		if (name == "direction")	return gvec3_to_str(this->getDirection());
		if (property_exists != NULL)
		{
			*property_exists = false;
		}
		return "";
	}
	
	bool Space3DObject::setProperty(chstr name, chstr value)
	{
		if		(name == "position")	this->setPosition(str_to_gvec3(value));
		else if	(name == "direction")	this->setDirection(str_to_gvec3(value));
		else return false;
		return true;
	}

}


