/// @file
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "aprilparticle.h"
#include "Util.h"

namespace aprilparticle
{
	hstr generateName(chstr prefix)
	{
		static hmap<hstr, int> counters;
		int count = counters[prefix] + 1;
		counters[prefix] = count;
		return prefix + "_" + hstr(count);
	}
	
	hstr gvec2_to_hstr(gvec2 vector)
	{
		return hsprintf("%f" APRILPARTICLE_VECTOR_SEPARATOR "%f", vector.x, vector.y);
	}

	hstr gvec3_to_hstr(gvec3 vector)
	{
		return hsprintf("%f" APRILPARTICLE_VECTOR_SEPARATOR "%f" APRILPARTICLE_VECTOR_SEPARATOR "%f", vector.x, vector.y, vector.z);
	}

	gvec2 hstr_to_gvec2(chstr string)
	{
		harray<hstr> data = string.split(APRILPARTICLE_VECTOR_SEPARATOR);
		if (data.size() != 2)
		{
			throw hl_exception("Error! Cannot convert string '" + string + "' to gtypes::Vector2.");
		}
		return gvec2(data[0], data[1]);
	}

	gvec3 hstr_to_gvec3(chstr string)
	{
		harray<hstr> data = string.split(APRILPARTICLE_VECTOR_SEPARATOR);
		if (data.size() != 3)
		{
			throw hl_exception("Error! Cannot convert string '" + string + "' to gtypes::Vector3.");
		}
		return gvec3(data[0], data[1], data[2]);
	}

}
