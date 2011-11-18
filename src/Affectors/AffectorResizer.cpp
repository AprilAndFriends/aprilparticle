/// @file
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "AffectorResizer.h"
#include "Particle.h"
#include "Util.h"

namespace aprilparticle
{
	namespace Affectors
	{
		Resizer::Resizer(chstr name) : Affector(name)
		{
			this->startSize.set(1.0f, 1.0f);
			this->endSize.set(0.0f, 0.0f);
		}

		Resizer::Resizer(gvec2 startSize, gvec2 endSize, chstr name) : Affector(name)
		{
			this->startSize = startSize;
			this->endSize = endSize;
		}

		Resizer::~Resizer()
		{
		}
		
		hstr Resizer::getProperty(chstr name, bool* property_exists)
		{
			if (property_exists != NULL)
			{
				*property_exists = true;
			}
			if (name == "start_size")	return gvec2_to_str(this->getStartSize());
			if (name == "end_size")		return gvec2_to_str(this->getEndSize());
			return Affector::getProperty(name, property_exists);
		}

		bool Resizer::setProperty(chstr name, chstr value)
		{
			if		(name == "start_size")	this->setStartSize(str_to_gvec2(value));
			else if	(name == "end_size")	this->setEndSize(str_to_gvec2(value));
			else return Affector::setProperty(name, value);
			return true;
		}

		void Resizer::update(Particle* particle, float k, gvec3& movement)
		{
			this->_ratio = particle->getLifeProgressRatio();
			particle->size = this->startSize * (1.0f - this->_ratio) + this->endSize * this->_ratio;
		}

	}

}

