/// @file
/// @version 3.0
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
		hmap<hstr, PropertyDescription> Scaler::_propertyDescriptions;

		Scaler::Scaler(chstr name) : Affector(name), _ratio(0.0f)
		{
			this->type = "Scaler";
			this->startScale = 1.0f;
			this->endScale = 0.0f;
		}

		Scaler::Scaler(float startScale, float endScale, chstr name) : Affector(name), _ratio(0.0f)
		{
			this->type = "Scaler";
			this->startScale = startScale;
			this->endScale = endScale;
		}

		Scaler::Scaler(const Scaler& other) : Affector(other), _ratio(0.0f)
		{
			this->startScale = other.startScale;
			this->endScale = other.endScale;
		}

		Scaler::~Scaler()
		{
		}
		
		Affector* Scaler::createInstance(chstr name)
		{
			return new Scaler(name);
		}

		hmap<hstr, PropertyDescription>& Scaler::getPropertyDescriptions() const
		{
			if (Scaler::_propertyDescriptions.size() == 0)
			{
				Scaler::_propertyDescriptions = Affector::getPropertyDescriptions();
				Scaler::_propertyDescriptions["start_scale"] = PropertyDescription("start_scale", PropertyDescription::Type::Float, 1.0f);
				Scaler::_propertyDescriptions["end_scale"] = PropertyDescription("end_scale", PropertyDescription::Type::Float, 0.0f);
			}
			return Scaler::_propertyDescriptions;
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

		void Scaler::update(Particle* particle, float timeDelta, gvec3f& movement)
		{
			this->_ratio = particle->getLifeProgressRatio();
			particle->scale = this->startScale * (1.0f - this->_ratio) + this->endScale * this->_ratio;
		}

	}

}

