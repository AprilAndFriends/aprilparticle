/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Color.h>
#include <hltypes/hmap.h>

#include "MultiColorAffector.h"
#include "Particle.h"

namespace aprilparticle
{
	MultiColorAffector::MultiColorAffector()
	{
		this->times += 0.0f;
		this->colors += APRIL_COLOR_WHITE;
		this->times += 1.0f;
		this->colors += april::Color(APRIL_COLOR_WHITE, 0);
		this->_size = 1;
	}
		
	MultiColorAffector::MultiColorAffector(hmap<float, april::Color> timeColors)
	{
		this->setTimeColors(timeColors);
	}

	MultiColorAffector::~MultiColorAffector()
	{
	}

	void MultiColorAffector::setTimeColors(hmap<float, april::Color> timeColors)
	{
		this->colors.clear();
		this->times = timeColors.keys().sorted();
		foreach (float, it, this->times)
		{
			this->colors += timeColors[*it];
		}
		this->_size = this->times.size() - 1;
	}
		
	void MultiColorAffector::addTimeColor(float time, april::Color color)
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

	void MultiColorAffector::update(Particle* particle, float k)
	{
		this->_ratio = particle->getLifeProgressRatio();
		for (this->_i = 0; this->_i < this->_size; this->_i++)
		{
			if (is_in_range(this->_ratio, this->times[this->_i], this->times[this->_i + 1]))
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
			particle->color = this->colors[this->_i];
		}
	}
		
	void MultiColorAffector::draw()
	{
	}

}

