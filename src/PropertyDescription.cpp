/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "aprilparticle.h"
#include "PropertyDescription.h"

namespace aprilparticle
{
	HL_ENUM_CLASS_DEFINE(PropertyDescription::Type,
	(
		HL_ENUM_DEFINE(PropertyDescription::Type, Int);
		HL_ENUM_DEFINE(PropertyDescription::Type, Float);
		HL_ENUM_DEFINE(PropertyDescription::Type, Char);
		HL_ENUM_DEFINE(PropertyDescription::Type, UChar);
		HL_ENUM_DEFINE(PropertyDescription::Type, Bool);
		HL_ENUM_DEFINE(PropertyDescription::Type, String);
		HL_ENUM_DEFINE(PropertyDescription::Type, Enum);
		HL_ENUM_DEFINE(PropertyDescription::Type, Grectf);
		HL_ENUM_DEFINE(PropertyDescription::Type, Gvec2f);
		HL_ENUM_DEFINE(PropertyDescription::Type, Gvec3f);
		HL_ENUM_DEFINE(PropertyDescription::Type, Color);
		HL_ENUM_DEFINE(PropertyDescription::Type, RangeFloat);
		HL_ENUM_DEFINE(PropertyDescription::Type, RangeGvec2f);
		HL_ENUM_DEFINE(PropertyDescription::Type, RangeGvec3f);
		HL_ENUM_DEFINE(PropertyDescription::Type, TimingColor);
		HL_ENUM_DEFINE(PropertyDescription::Type, TimingFloat);
		HL_ENUM_DEFINE(PropertyDescription::Type, TimingGvec2f);
	));

	PropertyDescription::PropertyDescription()
	{
		this->type = Type::String;
		this->arrayData = false;
	}

	PropertyDescription::PropertyDescription(chstr name, Type type, chstr defaultValue, bool arrayData)
	{
		this->name = name;
		this->type = type;
		this->defaultValue = defaultValue;
		this->arrayData = arrayData;
	}

}
