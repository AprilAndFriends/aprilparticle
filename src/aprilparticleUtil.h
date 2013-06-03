/// @file
/// @author  Boris Mikic
/// @version 2.0
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

#define RAND_RANGE(name) (this->min ## name != this->max ## name ? this->min ## name + (this->max ## name - this->min ## name) * hrandf(1.0f) : this->min ## name)
#define TRY_SET_TYPE(value, name) if (value == #name) this->setType(name)
#define TRY_GET_TYPE(value, name) if (value == name) return #name;
#define GET_RANGE(name, func) (this->getMin ## name() != this->getMax ## name() ? \
	func(this->getMin ## name()) + aprilparticle::SeparatorRange + func(this->getMax ## name()) : \
	func(this->getMin ## name()))

namespace aprilparticle
{
	hstr generateName(chstr prefix);
	hstr gvec2_to_hstr(gvec2 vector);
	hstr gvec3_to_hstr(gvec3 vector);
	gvec2 hstr_to_gvec2(chstr string);
	gvec3 hstr_to_gvec3(chstr string);
	
}

#endif
