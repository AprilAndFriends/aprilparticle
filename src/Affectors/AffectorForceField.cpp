/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <math.h>

#include <april/RenderSystem.h>
#include <gtypes/Vector3.h>
#include <hltypes/util.h>

#include "AffectorForceField.h"
#include "Particle.h"
#include "System.h"
#include "Util.h"

#define VERTEX_COUNT 91 // you can't touch this

namespace aprilparticle
{
	// optimizations
	april::ColoredVertex u[VERTEX_COUNT];
	april::ColoredVertex v[VERTEX_COUNT];
	april::ColoredVertex w[VERTEX_COUNT];
	april::ColoredVertex arrow[2];
	gvec3 ut;
	gvec3 vt;
	gvec3 wt;

	void initForceField()
	{
		for (int i = 0; i < VERTEX_COUNT; i++)
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
			this->_squaredLength = (this->position + this->system->getPosition() - particle->position).squaredLength();
			if (this->_squaredLength <= this->radius * this->radius)
			{
				this->_factor = (this->radius - sqrt(this->_squaredLength)) / this->radius;
				movement += this->direction * (RAND_RANGE(Randomness) * this->_factor * this->_factor * k);
			}
		}
		
		void ForceField::draw()
		{
			float length = this->direction.length();
			for (int i = 0; i < VERTEX_COUNT; i++)
			{
				u[i] = this->position + this->system->getPosition() + ut * length;
				v[i] = this->position + this->system->getPosition() + vt * length;
				w[i] = this->position + this->system->getPosition() + wt * length;
			}
			arrow[0] = this->position + this->system->getPosition();
			arrow[1] = this->position + this->system->getPosition() + this->direction;
			april::rendersys->render(april::LineStrip, u, VERTEX_COUNT);
			april::rendersys->render(april::LineStrip, v, VERTEX_COUNT);
			april::rendersys->render(april::LineStrip, w, VERTEX_COUNT);
			april::rendersys->render(april::LineStrip, arrow, 2);
		}

	}

}
