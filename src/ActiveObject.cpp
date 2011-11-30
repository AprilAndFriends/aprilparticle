/// @file
/// @author  Boris Mikic
/// @version 1.4
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
#include "aprilparticle.h"
#include "Util.h"

namespace aprilparticle
{
	ActiveObject::ActiveObject(chstr name)
	{
		this->name = (name == "" ? generateName("ActiveObject") : name);
		this->position.set(0.0f, 0.0f, 0.0f);
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
		if (name == "name")		return this->getName();
		if (name == "position")	return gvec3_to_hstr(this->getPosition());
		if (name == "visible")	return this->isVisible();
		if (name == "enabled")	return this->isEnabled();
		if (property_exists != NULL)
		{
			*property_exists = false;
		}
		return "";
		return "";
	}
	
	bool ActiveObject::setProperty(chstr name, chstr value)
	{
		if		(name == "name")		this->setName(value);
		else if	(name == "position")	this->setPosition(hstr_to_gvec3(value));
		else if	(name == "visible")		this->setVisible(value);
		else if	(name == "enabled")		this->setEnabled(value);
		else
		{
			aprilparticle::log("WARNING! Property '" + name + "' does not exist in " + this->name + "!");
			return false;
		}
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


