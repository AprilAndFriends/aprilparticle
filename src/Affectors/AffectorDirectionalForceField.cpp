/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <math.h>

#include <april/RenderSystem.h>
#include <gtypes/Vector3.h>

#include "AffectorDirectionalForceField.h"
#include "Particle.h"

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

	void initDirectionalForceField()
	{
		for (int i = 0; i < VERTEX_COUNT; i++)
		{
			ut.x = sin(i * 0.069777f);
			ut.y = cos(i * 0.069777f);
			vt.y = cos(i * 0.069777f);
			vt.z = sin(i * 0.069777f);
			wt.x = cos(i * 0.069777f);
			wt.z = sin(i * 0.069777f);
			u[i].color = 0xFFFFFFFF;
			v[i].color = 0xFFFFFFFF;
			w[i].color = 0xFFFFFFFF;
		}
		arrow[0].color = 0xFFFFFFFF;
		arrow[1].color = 0xFFFFFFFF;
	}

	namespace Affectors
	{
		DirectionalForceField::DirectionalForceField()
		{
			this->position = gvec3(0.0f, 0.0f, 0.0f);
			this->direction = gvec3(0.0f, 0.0f, 1.0f);
			this->force = 0.0f;
		}
		
		DirectionalForceField::DirectionalForceField(gvec3 position, gvec3 direction, float force)
		{
			this->position = position;
			this->direction = direction;
			this->force = force;
		}

		DirectionalForceField::~DirectionalForceField()
		{
		}
		
		hstr DirectionalForceField::getProperty(chstr name, bool* property_exists)
		{
			if (property_exists != NULL)
			{
				*property_exists = true;
			}
			if (name == "x")		return this->getX();
			if (name == "y")		return this->getY();
			if (name == "z")		return this->getZ();
			if (name == "dir_x")	return this->getDirectionX();
			if (name == "dir_y")	return this->getDirectionY();
			if (name == "dir_z")	return this->getDirectionZ();
			if (name == "force")	return this->getForce();
			return Affector::getProperty(name, property_exists);
		}

		bool DirectionalForceField::setProperty(chstr name, chstr value)
		{
			if		(name == "x")		this->setX(value);
			else if	(name == "y")		this->setY(value);
			else if	(name == "z")		this->setZ(value);
			else if	(name == "dir_x")	this->setDirectionX(value);
			else if	(name == "dir_y")	this->setDirectionY(value);
			else if	(name == "dir_z")	this->setDirectionZ(value);
			else if	(name == "force")	this->setForce(value);
			else return Affector::setProperty(name, value);
			return true;
		}

		void DirectionalForceField::update(Particle* particle, float k)
		{
			this->_length = (particle->position - this->position).length();
			particle->position += this->direction * (this->force - this->_length / this->force) *
				(this->force / (this->_length * this->_length * particle->direction.length() + 1.0f) - particle->speed) * k;
		}
		
		void DirectionalForceField::draw()
		{
			for (int i = 0; i < VERTEX_COUNT; i++)
			{
				u[i] = this->position + ut * this->force;
				v[i] = this->position + vt * this->force;
				w[i] = this->position + wt * this->force;
			}
			arrow[0] = this->position;
			arrow[1] = this->position + this->direction * this->force;
			april::rendersys->render(april::LineStrip, u, VERTEX_COUNT);
			april::rendersys->render(april::LineStrip, v, VERTEX_COUNT);
			april::rendersys->render(april::LineStrip, w, VERTEX_COUNT);
			april::rendersys->render(april::LineStrip, arrow, 2);
		}

	}

}

