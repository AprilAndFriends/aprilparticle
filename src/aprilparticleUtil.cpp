/// @file
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "aprilparticle.h"
#include "aprilparticleUtil.h"

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
		return hsprintf("%f%c%f", vector.x, aprilparticle::SeparatorVector, vector.y);
	}

	hstr gvec3_to_hstr(gvec3 vector)
	{
		return hsprintf("%f%c%f%c%f", vector.x, aprilparticle::SeparatorVector, vector.y, aprilparticle::SeparatorVector, vector.z);
	}

	gvec2 hstr_to_gvec2(chstr string)
	{
		harray<hstr> data = string.split(aprilparticle::SeparatorVector);
		if (data.size() != 2)
		{
			throw hl_exception("Cannot convert string '" + string + "' to gtypes::Vector2.");
		}
		return gvec2(data[0].trim(), data[1].trim());
	}

	gvec3 hstr_to_gvec3(chstr string)
	{
		harray<hstr> data = string.split(aprilparticle::SeparatorVector);
		if (data.size() != 3)
		{
			throw hl_exception("Cannot convert string '" + string + "' to gtypes::Vector3.");
		}
		return gvec3(data[0].trim(), data[1].trim(), data[2].trim());
	}

}
