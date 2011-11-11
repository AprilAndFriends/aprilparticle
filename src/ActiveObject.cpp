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

#include "ActiveObject.h"
#include "Affector.h"
#include "Util.h"

namespace aprilparticle
{
	ActiveObject::ActiveObject(chstr name) : Space3DObject()
	{
		this->name = (name == "" ? generateName("ActiveObject") : name);
		this->visible = true;
		this->enabled = true;
	}
	
	ActiveObject::~ActiveObject()
	{
	}

	hstr ActiveObject::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "name")		return getName();
		if (name == "visible")	return isVisible();
		if (name == "enabled")	return isEnabled();
		return Space3DObject::getProperty(name, property_exists);
	}
	
	bool ActiveObject::setProperty(chstr name, chstr value)
	{
		if (name == "name")			setName(value);
		else if (name == "visible")	setVisible(value);
		else if (name == "enabled")	setEnabled(value);
		else return Space3DObject::setProperty(name, value);
		return true;
	}

	Affector* ActiveObject::getAffector(chstr name)
	{
		foreach (Affector*, it, this->affectors)
		{
			if ((*it)->getName() == name)
			{
				return (*it);
			}
		}
		return NULL;
	}

}


