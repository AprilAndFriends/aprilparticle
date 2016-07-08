/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "Particle.h"
#include "AffectorScaler.h"

namespace aprilparticle
{
	namespace Affectors
	{
		harray<PropertyDescription> Scaler::_propertyDescriptions;

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

		harray<PropertyDescription> Scaler::getPropertyDescriptions() const
		{
			if (Scaler::_propertyDescriptions.size() == 0)
			{
				Scaler::_propertyDescriptions += PropertyDescription("start_scale", PropertyDescription::FLOAT);
				Scaler::_propertyDescriptions += PropertyDescription("end_scale", PropertyDescription::FLOAT);
			}
			return (Affector::getPropertyDescriptions() + Scaler::_propertyDescriptions);
		}

		hstr Scaler::getProperty(chstr name)
		{
			if (name == "start_scale")	return this->getStartScale();
			if (name == "end_scale")	return this->getEndScale();
			return Affector::getProperty(name);
		}

		bool Scaler::setProperty(chstr name, chstr value)
		{
			if		(name == "start_scale")	this->setStartScale(value);
			else if	(name == "end_scale")	this->setEndScale(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void Scaler::update(Particle* particle, float timeDelta, gvec3& movement)
		{
			this->_ratio = particle->getLifeProgressRatio();
			particle->scale = this->startScale * (1.0f - this->_ratio) + this->endScale * this->_ratio;
		}

	}

}

