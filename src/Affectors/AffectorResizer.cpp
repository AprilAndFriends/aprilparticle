/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/aprilUtil.h>

#include "AffectorResizer.h"
#include "Particle.h"

namespace aprilparticle
{
	namespace Affectors
	{
		harray<PropertyDescription> Resizer::_propertyDescriptions;

		Resizer::Resizer(chstr name) : Affector(name)
		{
			this->startSize.set(1.0f, 1.0f);
			this->endSize.set(0.0f, 0.0f);
		}

		Resizer::Resizer(cgvec2 startSize, cgvec2 endSize, chstr name) : Affector(name)
		{
			this->startSize = startSize;
			this->endSize = endSize;
		}

		Resizer::~Resizer()
		{
		}
		
		Affector* Resizer::createInstance(chstr name)
		{
			return new Resizer(name);
		}

		harray<PropertyDescription> Resizer::getPropertyDescriptions() const
		{
			if (Resizer::_propertyDescriptions.size() == 0)
			{
				Resizer::_propertyDescriptions += PropertyDescription("start_size", PropertyDescription::Type::Gvec2);
				Resizer::_propertyDescriptions += PropertyDescription("end_size", PropertyDescription::Type::Gvec2);
			}
			return (Affector::getPropertyDescriptions() + Resizer::_propertyDescriptions);
		}

		hstr Resizer::getProperty(chstr name)
		{
			if (name == "start_size")	return april::gvec2ToHstr(this->getStartSize());
			if (name == "end_size")		return april::gvec2ToHstr(this->getEndSize());
			return Affector::getProperty(name);
		}

		bool Resizer::setProperty(chstr name, chstr value)
		{
			if		(name == "start_size")	this->setStartSize(april::hstrToGvec2(value));
			else if	(name == "end_size")	this->setEndSize(april::hstrToGvec2(value));
			else return Affector::setProperty(name, value);
			return true;
		}

		void Resizer::update(Particle* particle, float timeDelta, gvec3& movement)
		{
			this->_ratio = particle->getLifeProgressRatio();
			particle->size = this->startSize * (1.0f - this->_ratio) + this->endSize * this->_ratio;
		}

	}

}

