/// @file
/// @version 3.0
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
		hmap<hstr, PropertyDescription> Resizer::_propertyDescriptions;

		Resizer::Resizer(chstr name) :
			Affector(name),
			_ratio(0.0f)
		{
			this->type = "Resizer";
			this->startSize.set(1.0f, 1.0f);
			this->endSize.set(0.0f, 0.0f);
		}

		Resizer::Resizer(cgvec2f startSize, cgvec2f endSize, chstr name) :
			Affector(name),
			_ratio(0.0f)
		{
			this->type = "Resizer";
			this->startSize = startSize;
			this->endSize = endSize;
		}

		Resizer::Resizer(const Resizer& other) :
			Affector(other),
			_ratio(0.0f)
		{
			this->startSize = other.startSize;
			this->endSize = other.endSize;
		}

		Resizer::~Resizer()
		{
		}
		
		Affector* Resizer::createInstance(chstr name)
		{
			return new Resizer(name);
		}

		hmap<hstr, PropertyDescription>& Resizer::getPropertyDescriptions() const
		{
			if (Resizer::_propertyDescriptions.size() == 0)
			{
				Resizer::_propertyDescriptions = Affector::getPropertyDescriptions();
				Resizer::_propertyDescriptions["start_size"] = PropertyDescription("start_size", PropertyDescription::Type::Gvec2f, april::gvec2ToHstr<float>(gvec2f(1.0f, 1.0f)));
				Resizer::_propertyDescriptions["end_size"] = PropertyDescription("end_size", PropertyDescription::Type::Gvec2f, april::gvec2ToHstr<float>(gvec2f(0.0f, 0.0f)));
			}
			return Resizer::_propertyDescriptions;
		}

		hstr Resizer::getProperty(chstr name)
		{
			if (name == "start_size")	return april::gvec2ToHstr<float>(this->getStartSize());
			if (name == "end_size")		return april::gvec2ToHstr<float>(this->getEndSize());
			return Affector::getProperty(name);
		}

		bool Resizer::setProperty(chstr name, chstr value)
		{
			if		(name == "start_size")	this->setStartSize(april::hstrToGvec2<float>(value));
			else if	(name == "end_size")	this->setEndSize(april::hstrToGvec2<float>(value));
			else return Affector::setProperty(name, value);
			return true;
		}

		void Resizer::update(Particle* particle, float timeDelta, gvec3f& movement)
		{
			this->_ratio = particle->getLifeProgressRatio();
			particle->size = this->startSize * (1.0f - this->_ratio) + this->endSize * this->_ratio;
		}

	}

}

