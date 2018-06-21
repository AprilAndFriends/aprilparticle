/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <math.h>

#include <april/RenderSystem.h>
#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>

#include "AffectorForceField.h"
#include "aprilparticleUtil.h"
#include "Particle.h"
#include "Space.h"

#define VERTEX_COUNT 91 // you can't touch this

namespace aprilparticle
{
	// optimizations
	static april::ColoredVertex u[VERTEX_COUNT];
	static april::ColoredVertex v[VERTEX_COUNT];
	static april::ColoredVertex w[VERTEX_COUNT];
	static april::ColoredVertex arrow[2];
	static gvec3f ut;
	static gvec3f vt;
	static gvec3f wt;

	void initForceField()
	{
		for_iter (i, 0, VERTEX_COUNT)
		{
			ut.set(sin(i * 0.069777f), cos(i * 0.069777f), 0.0f);
			vt.set(0.0f, cos(i * 0.069777f), sin(i * 0.069777f));
			wt.set(cos(i * 0.069777f), 0.0f, sin(i * 0.069777f));
			u[i].color = 0xFFFFFFFF;
			v[i].color = 0xFFFFFFFF;
			w[i].color = 0xFFFFFFFF;
		}
		arrow[0].color = 0xFFFFFFFF;
		arrow[1].color = 0xFFFFFFFF;
	}

	namespace Affectors
	{
		hmap<hstr, PropertyDescription> ForceField::_propertyDescriptions;

		ForceField::ForceField(chstr name) :
			Space(name),
			_factor(0.0f),
			_squaredLength(0.0f)
		{
			this->type = "ForceField";
			this->direction.set(0.0f, 0.0f, 1.0f);
		}
		
		ForceField::ForceField(cgvec3f position, float radius, cgvec3f direction, chstr name) :
			Space(position, radius, name),
			_factor(0.0f),
			_squaredLength(0.0f)
		{
			this->type = "ForceField";
			this->direction = direction;
		}

		ForceField::ForceField(const ForceField& other) :
			Space(other),
			_factor(0.0f),
			_squaredLength(0.0f)
		{
			this->direction = other.direction;
		}

		ForceField::~ForceField()
		{
		}
		
		Affector* ForceField::createInstance(chstr name)
		{
			return new ForceField(name);
		}

		hmap<hstr, PropertyDescription>& ForceField::getPropertyDescriptions() const
		{
			if (ForceField::_propertyDescriptions.size() == 0)
			{
				ForceField::_propertyDescriptions = Space::getPropertyDescriptions();
				ForceField::_propertyDescriptions["direction"] = PropertyDescription("direction", PropertyDescription::Type::Gvec3f, april::gvec3fToHstr(gvec3f(0.0f, 0.0f, 1.0f)));
			}
			return ForceField::_propertyDescriptions;
		}

		hstr ForceField::getProperty(chstr name)
		{
			if (name == "direction")	return april::gvec3fToHstr(this->getDirection());
			return Space::getProperty(name);
		}

		bool ForceField::setProperty(chstr name, chstr value)
		{
			if		(name == "direction")	this->setDirection(april::hstrToGvec3f(value));
			else return Space::setProperty(name, value);
			return true;
		}

		void ForceField::update(Particle* particle, float timeDelta, gvec3f& movement)
		{
			this->_squaredLength = (this->position + this->_space->getPosition() - particle->position).squaredLength();
			if (this->_squaredLength <= this->radius * this->radius)
			{
				this->_factor = (this->radius - hsqrt(this->_squaredLength)) / this->radius;
				movement += this->direction * (RAND_RANGE(Randomness) * this->_factor * this->_factor * timeDelta);
			}
		}

		// TODO - test
		void ForceField::draw()
		{
			float length = this->direction.length();
			gvec3f uVector = this->position + this->_space->getPosition() + ut * length;
			gvec3f vVector = this->position + this->_space->getPosition() + vt * length;
			gvec3f wVector = this->position + this->_space->getPosition() + wt * length;
			for_iter (i, 0, VERTEX_COUNT)
			{
				u[i].set(uVector);
				v[i].set(vVector);
				w[i].set(wVector);
			}
			arrow[0].set(this->position + this->_space->getPosition());
			arrow[1].set(this->position + this->_space->getPosition() + this->direction);
			april::rendersys->render(april::RenderOperation::LineStrip, u, VERTEX_COUNT);
			april::rendersys->render(april::RenderOperation::LineStrip, v, VERTEX_COUNT);
			april::rendersys->render(april::RenderOperation::LineStrip, w, VERTEX_COUNT);
			april::rendersys->render(april::RenderOperation::LineStrip, arrow, 2);
		}

	}

}

