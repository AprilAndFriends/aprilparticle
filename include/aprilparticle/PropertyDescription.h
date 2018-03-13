/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines a description for properties in objects.

#ifndef APRILPARTICLE_PROPERTY_DESCRIPTION_H
#define APRILPARTICLE_PROPERTY_DESCRIPTION_H

#include <hltypes/henum.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "aprilparticleExport.h"

namespace aprilparticle
{
	class aprilparticleExport PropertyDescription
	{
	public:
		HL_ENUM_CLASS_PREFIX_DECLARE(aprilparticleExport, Type,
		(
			HL_ENUM_DECLARE(Type, Int);
			HL_ENUM_DECLARE(Type, Float);
			HL_ENUM_DECLARE(Type, Char);
			HL_ENUM_DECLARE(Type, UChar);
			HL_ENUM_DECLARE(Type, Bool);
			HL_ENUM_DECLARE(Type, String);
			HL_ENUM_DECLARE(Type, Enum);
			HL_ENUM_DECLARE(Type, Grect);
			HL_ENUM_DECLARE(Type, Gvec2);
			HL_ENUM_DECLARE(Type, Gvec3);
			HL_ENUM_DECLARE(Type, Color);
			HL_ENUM_DECLARE(Type, RangeFloat);
			HL_ENUM_DECLARE(Type, RangeGvec2);
			HL_ENUM_DECLARE(Type, RangeGvec3);
			HL_ENUM_DECLARE(Type, TimingColor);
			HL_ENUM_DECLARE(Type, TimingFloat);
			HL_ENUM_DECLARE(Type, TimingGvec2);
		));

		PropertyDescription();
		PropertyDescription(chstr name, Type type, bool arrayData = false);

		HL_DEFINE_GET(hstr, name, Name);
		HL_DEFINE_GET(Type, type, Type);
		HL_DEFINE_IS(arrayData, ArrayData);

	protected:
		hstr name;
		Type type;
		bool arrayData;

	};

}
#endif
