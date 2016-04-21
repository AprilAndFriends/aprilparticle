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
/// Defines utility functions.

#ifndef APRILPARTICLE_UTIL_H
#define APRILPARTICLE_UTIL_H

#define RAND_RANGE(name) (this->min ## name != this->max ## name ? this->min ## name + (this->max ## name - this->min ## name) * hrandf(1.0f) : this->min ## name)
#define TRY_SET_TYPE(value, name) if (value == #name) this->setType(name)
#define TRY_GET_TYPE(value, name) if (value == name) return #name;
#define GET_RANGE(name, func) (this->getMin ## name() != this->getMax ## name() ? \
	func(this->getMin ## name()) + aprilparticle::SeparatorRange + func(this->getMax ## name()) : \
	func(this->getMin ## name()))

#endif
