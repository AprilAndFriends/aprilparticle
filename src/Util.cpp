/// @file
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

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
	
}
