/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "Particle.h"
#include "AffectorScaler.h"

namespace aprilparticle
{
	namespace Affectors
	{
		Scaler::Scaler(chstr name) : Affector(name)
		{
			this->startScale = 1.0f;
			this->endScale = 0.0f;
		}

		Scaler::Scaler(float startScale, float endScale, chstr name) : Affector(name)
		{
			this->startScale = startScale;
			this->endScale = endScale;
		}

		Scaler::~Scaler()
		{
		}
		
		Affector* Scaler::createInstance(chstr name)
		{
			return new Scaler(name);
		}

		hstr Scaler::getProperty(chstr name, bool* property_exists)
		{
			if (property_exists != NULL)
			{
				*property_exists = true;
			}
			if (name == "start_scale")	return this->getStartScale();
			if (name == "end_scale")	return this->getEndScale();
			return Affector::getProperty(name, property_exists);
		}

		bool Scaler::setProperty(chstr name, chstr value)
		{
			if		(name == "start_scale")	this->setStartScale(value);
			else if	(name == "end_scale")	this->setEndScale(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void Scaler::update(Particle* particle, float k, gvec3& movement)
		{
			this->_ratio = particle->getLifeProgressRatio();
			particle->scale = this->startScale * (1.0f - this->_ratio) + this->endScale * this->_ratio;
		}

	}

}

