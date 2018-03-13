/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/harray.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>
#include <april/aprilUtil.h>
#include "Affector.h"
#include "aprilparticle.h"
#include "aprilparticleUtil.h"

namespace aprilparticle
{
	hmap<hstr, PropertyDescription> Affector::_propertyDescriptions;

	Affector::Affector(chstr name)
	{
		this->name = (name == "" ? april::generateName("Affector") : name);
		this->minRandomness = 1.0f;
		this->maxRandomness = 1.0f;
		this->chaoticity.set(0.0f, 0.0f, 0.0f);
		this->space = NULL;
	}

	Affector::~Affector()
	{
	}

	hmap<hstr, PropertyDescription>& Affector::getPropertyDescriptions() const
	{
		if (Affector::_propertyDescriptions.size() == 0)
		{
			Affector::_propertyDescriptions["name"] = PropertyDescription("name", PropertyDescription::Type::String);
			Affector::_propertyDescriptions["randomness"] = PropertyDescription("randomness", PropertyDescription::Type::RangeFloat);
		}
		return Affector::_propertyDescriptions;
	}

	void Affector::setRandomness(float value)
	{
		this->minRandomness = value;
		this->maxRandomness = value;
	}

	void Affector::setRandomness(chstr value)
	{
		harray<hstr> data = value.split(aprilparticle::SeparatorRange);
		this->setRandomnessRange(data.first(), data.last());
	}

	void Affector::setRandomnessRange(float min, float max)
	{
		this->minRandomness = min;
		this->maxRandomness = max;
	}

	hstr Affector::getProperty(chstr name)
	{
		if (name == "name")			return this->getName();
		if (name == "randomness")	return GET_RANGE(Randomness, hstr);
		hlog::warnf(logTag, "Affector property '%s' does not exist in '%s'!", name.cStr(), this->name.cStr());
		return "";
	}
	
	bool Affector::setProperty(chstr name, chstr value)
	{
		if		(name == "name")		this->setName(value);
		else if	(name == "randomness")	this->setRandomness(value);
		else
		{
			hlog::warnf(logTag, "Affector property '%s' does not exist in '%s'!", name.cStr(), this->name.cStr());
			return false;
		}
		return true;
	}

}
