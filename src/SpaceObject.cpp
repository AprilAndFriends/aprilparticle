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
#include "SpaceObject.h"
#include "Affector.h"
#include "aprilparticle.h"
#include "aprilparticleUtil.h"

namespace aprilparticle
{
	hmap<hstr, PropertyDescription> SpaceObject::_propertyDescriptions;

	SpaceObject::SpaceObject(chstr name) : ActiveObject(name == "" ? april::generateName("SpaceObject") : name)
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

	hmap<hstr, PropertyDescription>& SpaceObject::getPropertyDescriptions() const
	{
		if (SpaceObject::_propertyDescriptions.size() == 0)
		{
			SpaceObject::_propertyDescriptions = ActiveObject::getPropertyDescriptions();
			SpaceObject::_propertyDescriptions["position"] = PropertyDescription("position", PropertyDescription::Type::Gvec3);
		}
		return SpaceObject::_propertyDescriptions;
	}

	hstr SpaceObject::getProperty(chstr name)
	{
		if (name == "position")	return april::gvec3ToHstr(this->getPosition());
		return ActiveObject::getProperty(name);
	}

	bool SpaceObject::setProperty(chstr name, chstr value)
	{
		if	(name == "position")	this->setPosition(april::hstrToGvec3(value));
		else return ActiveObject::setProperty(name, value);
		return true;
	}

}


