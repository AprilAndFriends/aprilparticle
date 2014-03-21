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
/// Defines macros for easier code generation of a timed affector.

#ifndef APRILPARTICLE_TIMED_TEMPLATE_H
#define APRILPARTICLE_TIMED_TEMPLATE_H

#define TIMED_TEMPLATE_INIT(first, last) \
	this->times += 0.0f; \
	this->values += first; \
	this->times += 1.0f; \
	this->values += last; \
	this->_size = 1;

#define TIMED_TEMPLATE_SET_TIMINGS \
	this->values.clear(); \
	this->times = value.keys().sorted(); \
	foreach (float, it, this->times) \
	{ \
		this->values += value[*it]; \
	} \
	this->_size = this->times.size() - 1;

#define TIMED_TEMPLATE_SET_TIMINGS_STRING(type, constructor) \
	harray<hstr> entries = value.split(aprilparticle::SeparatorValue); \
	harray<hstr> data; \
	hmap<float, type> timings; \
	foreach (hstr, it, entries) \
	{ \
		data = (*it).split(aprilparticle::SeparatorTiming); \
		if (data.size() == 2) \
		{ \
			timings[(float)data[0]] = constructor(data[1]); \
		} \
	} \
	if (timings.size() >= 2) \
	{ \
		this->setTimings(timings); \
	} \
	else \
	{ \
		this->times.clear(); \
		this->values.clear(); \
		hlog::warn(aprilparticle::logTag, "Incorrect usage of 'timings' for Affector!"); \
	}

#define TIMED_TEMPLATE_GET_TIMINGS_PROPERTY(prefix, suffix) \
	harray<hstr> result; \
	for_iter (i, 0, this->times.size()) \
	{ \
		result += hsprintf("%f%c%s", this->times[i], aprilparticle::SeparatorTiming, (prefix(this->values[i])suffix).c_str()); \
	} \
	return result.join(aprilparticle::SeparatorVector);

#define TIMED_TEMPLATE_ADD_TIMING \
	time = hclamp(time, 0.0f, 1.0f); \
	for_iterx (this->_i, 0, this->times.size()) \
	{ \
		if (time > this->times[this->_i]) \
		{ \
			break; \
		} \
	} \
	this->times.insert_at(this->_i, time); \
	this->values.insert_at(this->_i, value); \
	++this->_size;

#define TIMED_TEMPLATE_UPDATE(particle, var) \
	if (this->times.size() == 0) \
	{ \
		return; \
	} \
	this->_ratio = particle->getLifeProgressRatio(); \
	if (this->_ratio <= this->times.first()) \
	{ \
		particle->var = this->values.first(); \
	} \
	else \
	{ \
		for_iterx (this->_i, 0, this->_size) \
		{ \
			if (is_inside(this->_ratio, this->times[this->_i], this->times[this->_i + 1])) \
			{ \
				break; \
			} \
		} \
		if (this->_i < this->_size) \
		{ \
			this->_ratio = (this->times[this->_i + 1] - this->_ratio) / (this->times[this->_i + 1] - this->times[this->_i]); \
			particle->var = this->values[this->_i] * this->_ratio + this->values[this->_i + 1] * (1.0f - this->_ratio); \
		} \
		else \
		{ \
			particle->var = this->values.last(); \
		} \
	}


#endif
