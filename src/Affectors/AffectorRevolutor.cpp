/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <math.h>

#include <april/aprilUtil.h>
#include <gtypes/Matrix3.h>

#include "AffectorRevolutor.h"
#include "aprilparticleUtil.h"
#include "Particle.h"
#include "Space.h"

namespace aprilparticle
{
	namespace Affectors
	{
		static gmat3 _rotation;

		hmap<hstr, PropertyDescription> Revolutor::_propertyDescriptions;

		Revolutor::Revolutor(chstr name) : Space(name)
		{
			this->axis.set(0.0f, 1.0f, 0.0f);
			this->evolutionSpeed = 1.0f;
			this->setClockwise(true);
		}

		Revolutor::Revolutor(cgvec3 position, float radius, cgvec3 axis, float evolutionSpeed, bool clockwise, chstr name) : Space(position, radius, name)
		{
			this->axis = axis;
			this->evolutionSpeed = evolutionSpeed;
			this->setClockwise(clockwise);
		}
		
		Revolutor::~Revolutor()
		{
		}
		
		Affector* Revolutor::createInstance(chstr name)
		{
			return new Revolutor(name);
		}

		hmap<hstr, PropertyDescription>& Revolutor::getPropertyDescriptions() const
		{
			if (Revolutor::_propertyDescriptions.size() == 0)
			{
				Revolutor::_propertyDescriptions = Space::getPropertyDescriptions();
				Revolutor::_propertyDescriptions["axis"] = PropertyDescription("axis", PropertyDescription::Type::Gvec3);
				Revolutor::_propertyDescriptions["evolution_speed"] = PropertyDescription("evolution_speed", PropertyDescription::Type::Float);
				Revolutor::_propertyDescriptions["clockwise"] = PropertyDescription("clockwise", PropertyDescription::Type::Bool);
			}
			return Revolutor::_propertyDescriptions;
		}

		bool Revolutor::isClockwise() const
		{
			return (this->angle >= 0.0f);
		}

		void Revolutor::setClockwise(bool value)
		{
			this->angle = (value ? -360.0f : 360.0f);
		}

		hstr Revolutor::getProperty(chstr name)
		{
			if (name == "axis")				return april::gvec3ToHstr(this->getAxis());
			if (name == "evolution_speed")	return this->getEvolutionSpeed();
			if (name == "clockwise")		return this->isClockwise();
			return Space::getProperty(name);
		}

		bool Revolutor::setProperty(chstr name, chstr value)
		{
			if		(name == "axis")			this->setAxis(april::hstrToGvec3(value));
			else if	(name == "evolution_speed")	this->setEvolutionSpeed(value);
			else if	(name == "clockwise")		this->setClockwise(value);
			else return Space::setProperty(name, value);
			return true;
		}

		void Revolutor::update(Particle* particle, float timeDelta, gvec3& movement)
		{
			this->_position = this->position + this->space->getPosition();
			this->_direction = particle->position - this->_position;
			this->_squaredLength = this->_direction.squaredLength();
			if (this->_squaredLength < this->radius * this->radius)
			{
				_rotation.setRotation3D(this->axis, (1.0f - hsqrt(this->_squaredLength) / this->radius) * RAND_RANGE(Randomness) * this->evolutionSpeed * this->angle * timeDelta);
				movement += this->_position + _rotation * this->_direction - particle->position;
			}
		}

	}

}

