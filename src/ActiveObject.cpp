/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>
#include <april/aprilUtil.h>
#include "ActiveObject.h"
#include "Affector.h"
#include "aprilparticle.h"
#include "aprilparticleUtil.h"

namespace aprilparticle
{
	harray<PropertyDescription> ActiveObject::_propertyDescriptions;

	ActiveObject::ActiveObject(chstr name)
	{
		this->name = (name == "" ? april::generateName("ActiveObject") : name);
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

	harray<PropertyDescription> ActiveObject::getPropertyDescriptions()
	{
		if (ActiveObject::_propertyDescriptions.size() == 0)
		{
			ActiveObject::_propertyDescriptions += PropertyDescription("name", PropertyDescription::STRING);
			ActiveObject::_propertyDescriptions += PropertyDescription("visible", PropertyDescription::BOOL);
			ActiveObject::_propertyDescriptions += PropertyDescription("enabled", PropertyDescription::BOOL);
		}
		return ActiveObject::_propertyDescriptions;
	}

	bool ActiveObject::hasProperty(chstr name)
	{
		harray<PropertyDescription> properties = this->getPropertyDescriptions();
		foreach (PropertyDescription, it, properties)
		{
			if ((*it).getName() == name)
			{
				return true;
			}
		}
		return false;
	}

	hstr ActiveObject::getProperty(chstr name)
	{
		if (name == "name")		return this->getName();
		if (name == "visible")	return this->isVisible();
		if (name == "enabled")	return this->isEnabled();
		return "";
	}
	
	bool ActiveObject::setProperty(chstr name, chstr value)
	{
		if		(name == "name")		this->setName(value);
		else if	(name == "visible")		this->setVisible(value);
		else if	(name == "enabled")		this->setEnabled(value);
		else
		{
			hlog::warnf(logTag, "Property '%s' does not exist in '%s'!", name.cStr(), this->name.cStr());
			return false;
		}
		return true;
	}

}
