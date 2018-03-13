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
	hmap<hstr, PropertyDescription> ActiveObject::_propertyDescriptions;

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

	hmap<hstr, PropertyDescription>& ActiveObject::getPropertyDescriptions() const
	{
		if (ActiveObject::_propertyDescriptions.size() == 0)
		{
			ActiveObject::_propertyDescriptions["name"] = PropertyDescription("name", PropertyDescription::Type::String);
			ActiveObject::_propertyDescriptions["visible"] = PropertyDescription("visible", PropertyDescription::Type::Bool);
			ActiveObject::_propertyDescriptions["enabled"] = PropertyDescription("enabled", PropertyDescription::Type::Bool);
		}
		return ActiveObject::_propertyDescriptions;
	}

	bool ActiveObject::hasProperty(chstr name)
	{
		return this->getPropertyDescriptions().hasKey(name);
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
