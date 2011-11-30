/// @file
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines macros for easier code generation of a timed affector.

#ifndef APRILPARTICLE_TIMED_TEMPLATE_H
#define APRILPARTICLE_TIMED_TEMPLATE_H

#define TIMED_TEMPLATE_INIT(name, first, last) \
	this->times += 0.0f; \
	this->name += first; \
	this->times += 1.0f; \
	this->name += last; \
	this->_size = 1;

#define TIMED_TEMPLATE_SET_TIMINGS(name) \
	this->name.clear(); \
	this->times = value.keys().sorted(); \
	foreach (float, it, this->times) \
	{ \
		this->name += value[*it]; \
	} \
	this->_size = this->times.size() - 1;

#define TIMED_TEMPLATE_SET_TIMINGS_STRING(type, constructor) \
	harray<hstr> entries = value.split(APRILPARTICLE_VALUE_SEPARATOR); \
	harray<hstr> data; \
	hmap<float, type> timings; \
	foreach (hstr, it, entries) \
	{ \
		data = (*it).split(APRILPARTICLE_TIMING_SEPARATOR); \
		if (data.size() == 2) \
		{ \
			timings[(float)data[0]] = constructor(data[1]); \
		} \
	} \
	this->setTimings(timings);

#define TIMED_TEMPLATE_GET_TIMINGS_PROPERTY(name, prefix, suffix) \
	harray<hstr> result; \
	for (int i = 0; i < this->times.size(); i++) \
	{ \
		result += hsprintf("%f" APRILPARTICLE_TIMING_SEPARATOR "%s", this->times[i], (prefix(this->name[i])suffix).c_str()); \
	} \
	return result.join(APRILPARTICLE_VECTOR_SEPARATOR);

#define TIMED_TEMPLATE_ADD_TIMING(name, value) \
	time = hclamp(time, 0.0f, 1.0f); \
	for (this->_i = 0; this->_i < this->times.size(); this->_i++) \
	{ \
		if (time > this->times[this->_i]) \
		{ \
			break; \
		} \
	} \
	this->times.insert_at(this->_i, time); \
	this->name.insert_at(this->_i, value); \
	this->_size++;

#define TIMED_TEMPLATE_UPDATE(particle, var, name) \
	this->_ratio = particle->getLifeProgressRatio(); \
	if (this->_ratio <= this->times.first()) \
	{ \
		particle->var = this->name.first(); \
	} \
	else \
	{ \
		for (this->_i = 0; this->_i < this->_size; this->_i++) \
		{ \
			if (is_inside(this->_ratio, this->times[this->_i], this->times[this->_i + 1])) \
			{ \
				break; \
			} \
		} \
		if (this->_i < this->_size) \
		{ \
			this->_ratio = (this->times[this->_i + 1] - this->_ratio) / (this->times[this->_i + 1] - this->times[this->_i]); \
			particle->var = this->name[this->_i] * this->_ratio + this->name[this->_i + 1] * (1.0f - this->_ratio); \
		} \
		else \
		{ \
			particle->var = this->name.last(); \
		} \
	}


#endif
