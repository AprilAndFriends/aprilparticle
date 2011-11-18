/// @file
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines utility functions.

#ifndef APRILPARTICLE_UTIL_H
#define APRILPARTICLE_UTIL_H

#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/hstring.h>

namespace aprilparticle
{
	hstr generateName(chstr prefix);
	hstr gvec2_to_str(gvec2 vector);
	hstr gvec3_to_str(gvec3 vector);
	gvec2 str_to_gvec2(chstr string);
	gvec3 str_to_gvec3(chstr string);
	
}

#endif
