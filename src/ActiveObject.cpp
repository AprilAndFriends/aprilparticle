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

#include "ActiveObject.h"
#include "Affector.h"
#include "aprilparticle.h"
#include "aprilparticleUtil.h"

namespace aprilparticle
{
	ActiveObject::ActiveObject(chstr name)
	{
		this->name = (name == "" ? generateName("ActiveObject") : name);
		this->visible = true;
		this->enabled = true;
	}
	
	ActiveObject::ActiveObject(const ActiveObject& other)
	{
		this->name = other.name;
		this->visible = other.visible;
		this->enabled = other.enabled;
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
		if (name == "visible")	return this->isVisible();
		if (name == "enabled")	return this->isEnabled();
		if (property_exists != NULL)
		{
			hlog::warnf(aprilparticle::logTag, "Property '%s' does not exist in '%s'!", name.c_str(), this->name.c_str());
			*property_exists = false;
		}
		return "";
	}
	
	bool ActiveObject::setProperty(chstr name, chstr value)
	{
		if		(name == "name")		this->setName(value);
		else if	(name == "visible")		this->setVisible(value);
		else if	(name == "enabled")		this->setEnabled(value);
		else
		{
			hlog::warnf(aprilparticle::logTag, "Property '%s' does not exist in '%s'!", name.c_str(), this->name.c_str());
			return false;
		}
		return true;
	}

}
