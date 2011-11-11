/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Color.h>
#include <hltypes/hmap.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "AffectorColorMultiChanger.h"
#include "aprilparticle.h"
#include "Particle.h"

#define KEYVALUE_SEPARATOR ":"

namespace aprilparticle
{
	namespace Affectors
	{
		ColorMultiChanger::ColorMultiChanger(chstr name) : Affector(name)
		{
			this->times += 0.0f;
			this->colors += APRIL_COLOR_WHITE;
			this->times += 1.0f;
			this->colors += april::Color(APRIL_COLOR_WHITE, 0);
			this->_size = 1;
		}
		
		ColorMultiChanger::ColorMultiChanger(hmap<float, april::Color> colorTimings, chstr name) : Affector(name)
		{
			this->setColorTimings(colorTimings);
		}

		ColorMultiChanger::~ColorMultiChanger()
		{
		}

		void ColorMultiChanger::setColorTimings(hmap<float, april::Color> value)
		{
			this->colors.clear();
			this->times = value.keys().sorted();
			foreach (float, it, this->times)
			{
				this->colors += value[*it];
			}
			this->_size = this->times.size() - 1;
		}
		
		void ColorMultiChanger::setColorTimings(chstr value)
		{
			harray<hstr> entries = value.split(APRILPARTICLE_VALUE_SEPARATOR);
			harray<hstr> data;
			hmap<float, april::Color> colorTimings;
			foreach (hstr, it, entries)
			{
				data = (*it).split(KEYVALUE_SEPARATOR);
				if (data.size() == 2)
				{
					colorTimings[(float)data[0]] = april::Color(data[1]);
				}
			}
			this->setColorTimings(colorTimings);
		}
		
		void ColorMultiChanger::addColorTiming(float time, april::Color color)
		{
			time = hclamp(time, 0.0f, 1.0f);
			for (this->_i = 0; this->_i < this->times.size(); this->_i++)
			{
				if (time > this->times[this->_i])
				{
					break;
				}
			}
			this->times.insert_at(this->_i, time);
			this->colors.insert_at(this->_i, color);
			this->_size++;
		}

		hstr ColorMultiChanger::getProperty(chstr name, bool* property_exists)
		{
			if (property_exists != NULL)
			{
				*property_exists = true;
			}
			if (name == "color_timings")
			{
				harray<hstr> result;
				for (int i = 0; i < this->times.size(); i++)
				{
					result += hsprintf("%f" KEYVALUE_SEPARATOR "%s", this->times[i], this->colors[i].hex());
				}
				return result.join(APRILPARTICLE_VALUE_SEPARATOR);
			}
			return Affector::getProperty(name, property_exists);
		}

		bool ColorMultiChanger::setProperty(chstr name, chstr value)
		{
			if (name == "color_timings")	this->setColorTimings(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void ColorMultiChanger::update(Particle* particle, float k)
		{
			this->_ratio = particle->getLifeProgressRatio();
			this->_size = this->times.size() - 1;
			for (this->_i = 0; this->_i < this->_size; this->_i++)
			{
				if (is_inside(this->_ratio, this->times[this->_i], this->times[this->_i + 1]))
				{
					break;
				}
			}
			if (this->_i < this->_size)
			{
				this->_ratio = (this->times[this->_i + 1] - this->_ratio) / (this->times[this->_i + 1] - this->times[this->_i]);
				particle->color = this->colors[this->_i] * this->_ratio + this->colors[this->_i + 1] * (1.0f - this->_ratio);
			}
			else
			{
				particle->color = this->colors.last();
			}
		}
		
	}

}

