/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

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
	static gvec3 ut;
	static gvec3 vt;
	static gvec3 wt;

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
		ForceField::ForceField(chstr name) : Space(name)
		{
			this->direction.set(0.0f, 0.0f, 1.0f);
		}
		
		ForceField::ForceField(gvec3 position, float radius, gvec3 direction, chstr name) : Space(position, radius, name)
		{
			this->direction = direction;
		}

		ForceField::~ForceField()
		{
		}
		
		Affector* ForceField::createInstance(chstr name)
		{
			return new ForceField(name);
		}

		hstr ForceField::getProperty(chstr name, bool* property_exists)
		{
			if (property_exists != NULL)
			{
				*property_exists = true;
			}
			if (name == "direction")	return gvec3_to_hstr(this->getDirection());
			return Space::getProperty(name, property_exists);
		}

		bool ForceField::setProperty(chstr name, chstr value)
		{
			if		(name == "direction")	this->setDirection(hstr_to_gvec3(value));
			else return Space::setProperty(name, value);
			return true;
		}

		void ForceField::update(Particle* particle, float k, gvec3& movement)
		{
			this->_squaredLength = (this->position + this->space->getPosition() - particle->position).squaredLength();
			if (this->_squaredLength <= this->radius * this->radius)
			{
				this->_factor = (this->radius - hsqrt(this->_squaredLength)) / this->radius;
				movement += this->direction * (RAND_RANGE(Randomness) * this->_factor * this->_factor * k);
			}
		}
		
		void ForceField::draw()
		{
			static float length;
			length = this->direction.length();
			for_iter (i, 0, VERTEX_COUNT)
			{
				u[i] = this->position + this->space->getPosition() + ut * length;
				v[i] = this->position + this->space->getPosition() + vt * length;
				w[i] = this->position + this->space->getPosition() + wt * length;
			}
			arrow[0] = this->position + this->space->getPosition();
			arrow[1] = this->position + this->space->getPosition() + this->direction;
			april::rendersys->render(april::RO_LINE_STRIP, u, VERTEX_COUNT);
			april::rendersys->render(april::RO_LINE_STRIP, v, VERTEX_COUNT);
			april::rendersys->render(april::RO_LINE_STRIP, w, VERTEX_COUNT);
			april::rendersys->render(april::RO_LINE_STRIP, arrow, 2);
		}

	}

}

