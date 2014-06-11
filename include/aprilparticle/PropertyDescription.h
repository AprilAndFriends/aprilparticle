/// @file
/// @version 2.2
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

#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "aprilparticleExport.h"

namespace aprilparticle
{
	class aprilparticleExport PropertyDescription
	{
	public:
		enum PropertyType
		{
			INT,
			FLOAT,
			CHAR,
			UCHAR,
			BOOL,
			STRING,
			ENUM,
			GRECT,
			GVEC2,
			GVEC3,
			HEXCOLOR,
			RANGE_FLOAT,
			RANGE_GVEC2,
			RANGE_GVEC3,
			TIMING_HEXCOLOR,
			TIMING_FLOAT,
			TIMING_GVEC2
		};

		PropertyDescription(chstr name, PropertyType type);
		~PropertyDescription();

		HL_DEFINE_GET(PropertyType, type, Type);
		HL_DEFINE_GET(hstr, name, Name);

	protected:
		hstr name;
		PropertyType type;

	};

}
#endif
