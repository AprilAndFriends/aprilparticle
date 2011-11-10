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

#include <gtypes/Constants.h>
#include <gtypes/Matrix3.h>
#include <gtypes/Matrix4.h>
#include <gtypes/Vector2.h>
#include <hltypes/hdeque.h>
#include <hltypes/util.h>

#include "Affector.h"
#include "aprilparticle.h"
#include "Emitter.h"
#include "Particle.h"
#include "System.h"
#include "Util.h"

#define DATA_SEPARATOR " "
#define RANDOMIZE(name) (this->min ## name < this->max ## name ? hrandf(this->min ## name, this->max ## name) : this->min ## name)
#define TRY_SET_TYPE(value, name) if (value == #name) this->setType(name)
#define TRY_GET_TYPE(value, name) if (value == name) return #name;

namespace aprilparticle
{
	gvec3 v[4]; // optimization

	Emitter::Emitter(chstr name) : ActiveObject(name == "" ? generateName("Emitter") : name)
	{
		this->timer = 0.0f;
		this->type = Point;
		this->dimensions.set(1.0f, 1.0f, 1.0f);
		this->blendMode = april::ADD;
		this->emissionRate = emissionRate;
		this->limit = 10;
		this->minLife = 1.0f;
		this->maxLife = 1.0f;
		this->minSize.set(1.0f, 1.0f);
		this->maxSize.set(1.0f, 1.0f);
		this->minScale = 1.0f;
		this->maxScale = 1.0f;
		this->minSpeed = 0.0f;
		this->maxSpeed = 0.0f;
		this->minAngle = 0.0f;
		this->maxAngle = 0.0f;
		this->texture = NULL;
		this->_triangleBatch = NULL;
		this->_setupTriangleBatch();
		this->system = NULL;
	}

	Emitter::~Emitter()
	{
		if (this->_triangleBatch != NULL)
		{
			delete [] this->_triangleBatch;
		}
		foreach_q (Particle*, it, this->particles)
		{
			delete (*it);
		}
	}

	void Emitter::setLife(float value)
	{
		this->minLife = value;
		this->maxLife = value;
	}

	void Emitter::setSize(gvec2 value)
	{
		this->minSize = value;
		this->maxSize = value;
	}

	void Emitter::setScale(float value)
	{
		this->minScale = value;
		this->maxScale = value;
	}

	void Emitter::setSpeed(float value)
	{
		this->minSpeed = value;
		this->maxSpeed = value;
	}

	void Emitter::setAngle(float value)
	{
		this->minAngle = value;
		this->maxAngle = value;
	}

	void Emitter::setLife(chstr value)
	{
		harray<hstr> data = value.split(DATA_SEPARATOR);
		this->setLifeRange(data.first(), data.last());
	}

	void Emitter::setSize(chstr value)
	{
		harray<hstr> data = value.split(DATA_SEPARATOR);
		this->setSizeRange(str_to_gvec2(data.first()), str_to_gvec2(data.last()));
	}

	void Emitter::setScale(chstr value)
	{
		harray<hstr> data = value.split(DATA_SEPARATOR);
		this->setScaleRange(data.first(), data.last());
	}

	void Emitter::setSpeed(chstr value)
	{
		harray<hstr> data = value.split(DATA_SEPARATOR);
		this->setSpeedRange(data.first(), data.last());
	}

	void Emitter::setAngle(chstr value)
	{
		harray<hstr> data = value.split(APRILPARTICLE_VALUE_SEPARATOR);
		this->setAngleRange(data.first(), data.last());
	}

	void Emitter::setLimit(int value)
	{
		if (this->limit != value)
		{
			this->limit = value;
			this->_setupTriangleBatch();
		}
	}

	void Emitter::_setupTriangleBatch()
	{
		if (this->_triangleBatch != NULL)
		{
			delete [] this->_triangleBatch;
		}
		this->_triangleBatch = new april::ColoredTexturedVertex[this->limit * 6];
		for (this->_i = 0; this->_i < this->limit; this->_i++)
		{
			this->_triangleBatch[this->_i * 6 + 0].u = 1.0f;		this->_triangleBatch[this->_i * 6 + 0].v = 1.0f;
			this->_triangleBatch[this->_i * 6 + 1].u = 0.0f;		this->_triangleBatch[this->_i * 6 + 1].v = 1.0f;
			this->_triangleBatch[this->_i * 6 + 2].u = 1.0f;		this->_triangleBatch[this->_i * 6 + 2].v = 0.0f;
			this->_triangleBatch[this->_i * 6 + 3].u = 0.0f;		this->_triangleBatch[this->_i * 6 + 3].v = 1.0f;
			this->_triangleBatch[this->_i * 6 + 4].u = 1.0f;		this->_triangleBatch[this->_i * 6 + 4].v = 0.0f;
			this->_triangleBatch[this->_i * 6 + 5].u = 0.0f;		this->_triangleBatch[this->_i * 6 + 5].v = 0.0f;
		}
	}

	void Emitter::setLifeRange(float min, float max)
	{
		this->minLife = min;
		this->maxLife = max;
	}

	void Emitter::setSizeRange(gvec2 min, gvec2 max)
	{
		this->minSize = min;
		this->maxSize = max;
	}

	void Emitter::setScaleRange(float min, float max)
	{
		this->minScale = min;
		this->maxScale = max;
	}

	void Emitter::setSpeedRange(float min, float max)
	{
		this->minSpeed = min;
		this->maxSpeed = max;
	}

	void Emitter::setAngleRange(float min, float max)
	{
		this->minAngle = min;
		this->maxAngle = max;
	}

	void Emitter::addAffector(Affector* affector)
	{
		this->affectors += affector;
	}
	
	void Emitter::removeAffector(Affector* affector)
	{
		this->affectors -= affector;
	}
	
	hstr Emitter::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "name")		return this->getName();
		if (name == "type")
		{
			Type value = this->getType();
			TRY_GET_TYPE(value, Point);
			TRY_GET_TYPE(value, Box);
			TRY_GET_TYPE(value, HollowBox);
			TRY_GET_TYPE(value, Sphere);
			TRY_GET_TYPE(value, HollowSphere);
			TRY_GET_TYPE(value, Cylinder);
			TRY_GET_TYPE(value, HollowCylinder);
			TRY_GET_TYPE(value, Circle);
			TRY_GET_TYPE(value, Ring);
			return "";
		}
		if (name == "dimensions")		return gvec3_to_str(this->getDimensions());
		return ActiveObject::getProperty(name, property_exists);
	}

	bool Emitter::setProperty(chstr name, chstr value)
	{
		if		(name == "name")		this->setName(value);
		else if	(name == "type")
		{
			TRY_SET_TYPE(value, Point);
			TRY_SET_TYPE(value, Box);
			TRY_SET_TYPE(value, HollowBox);
			TRY_SET_TYPE(value, Sphere);
			TRY_SET_TYPE(value, HollowSphere);
			TRY_SET_TYPE(value, Cylinder);
			TRY_SET_TYPE(value, HollowCylinder);
			TRY_SET_TYPE(value, Circle);
			TRY_SET_TYPE(value, Ring);
		}
		else if	(name == "dimensions")		this->setDimensions(str_to_gvec3(value));
		else if	(name == "blend_mode")
		{
			if		(value == "default")		this->setBlendMode(april::DEFAULT);
			else if	(value == "alpha_blend")	this->setBlendMode(april::ALPHA_BLEND);
			else if	(value == "add")			this->setBlendMode(april::ADD);
			else if	(value == "subtract")		this->setBlendMode(april::SUBTRACT);
		}
		else if	(name == "emission_rate")	this->setEmissionRate(value);
		else if	(name == "limit")			this->setLimit(value);
		else if	(name == "life")			this->setLife(value);
		else if	(name == "size")			this->setSize(value);
		else if	(name == "scale")			this->setScale(value);
		else if	(name == "speed")			this->setSpeed(value);
		else if	(name == "angle")			this->setAngle(value);
		else return ActiveObject::setProperty(name, value);
		return true;
	}

	void Emitter::_createNewParticle()
	{
		//this->_pos = 
		switch (this->type)
		{
			case Point:
				this->_pos = this->position;
				break;
			case Box:
				this->_pos.x = this->position.x + hrandf(1.0f) * this->dimensions.x - this->dimensions.x * 0.5f;
				this->_pos.y = this->position.y + hrandf(1.0f) * this->dimensions.y - this->dimensions.y * 0.5f;
				this->_pos.z = this->position.z + hrandf(1.0f) * this->dimensions.z - this->dimensions.z * 0.5f;
				break;
			case HollowBox:
				this->_pos.x = this->position.x + hrand(2) * this->dimensions.x - this->dimensions.x * 0.5f;
				this->_pos.y = this->position.y + hrand(2) * this->dimensions.y - this->dimensions.y * 0.5f;
				this->_pos.z = this->position.z + hrand(2) * this->dimensions.z - this->dimensions.z * 0.5f;
				break;
			case Sphere:
				// TODO
				this->_rho = hrandf(1.0f);
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				this->_S = this->_rho * sin(this->_phi);

				this->_pos.x = this->position.x + this->_S * cos(this->_theta) * this->dimensions.x * 0.5f;
				this->_pos.y = this->position.y + this->_S * sin(this->_theta) * this->dimensions.y * 0.5f;
				this->_pos.z = this->position.z + this->_rho * cos(this->_phi) * this->dimensions.z * 0.5f;
				break;
			case HollowSphere:
				// TODO
				this->_rho = 1.0f;
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				this->_S = this->_rho * sin(this->_phi);

				this->_pos.x = this->position.x + this->_S * cos(this->_theta) * this->dimensions.x * 0.5f;
				this->_pos.y = this->position.y + this->_S * sin(this->_theta) * this->dimensions.y * 0.5f;
				this->_pos.z = this->position.z + this->_rho * cos(this->_phi) * this->dimensions.z * 0.5f;
				break;
			case Cylinder:
				// TODO
				this->_rho = hrandf(1.0f);
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				this->_S = this->_rho * sin(this->_phi);
					
				this->_pos.x = this->position.x + this->_S * cos(this->_theta) * this->dimensions.x * 0.5f;
				this->_pos.y = this->position.y + hrandf(1.0f) * this->dimensions.y - this->dimensions.y * 0.5f;
				this->_pos.z = this->position.z + this->_rho * cos(this->_phi) * this->dimensions.z * 0.5f;
				break;
			case HollowCylinder:
				// TODO
				this->_rho = 1.0f;
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				this->_S = this->_rho * sin(this->_phi);
					
				this->_pos.x = this->position.x + this->_S * cos(this->_theta) * this->dimensions.x * 0.5f;
				this->_pos.y = this->position.y + hrandf(1.0f) * this->dimensions.y - this->dimensions.y * 0.5f;
				this->_pos.z = this->position.z + this->_rho * cos(this->_phi) * this->dimensions.z * 0.5f;
				break;
			case Circle:
				// TODO
				this->_pos = this->position;
				break;
			case Ring:
				// TODO
				this->_pos = this->position;
				break;
		}
		if (this->system != NULL)
		{
			this->_pos += this->system->getPosition();
		}
		this->_particle = new Particle(this->_pos, this->direction, RANDOMIZE(Life), gvec2(RANDOMIZE(Size.x), RANDOMIZE(Size.y)),
			RANDOMIZE(Scale), RANDOMIZE(Speed), RANDOMIZE(Angle));
		this->particles += this->_particle;
		foreach (Affector*, it, this->affectors)
		{
			(*it)->update(this->_particle, 0.0f);
		}
	}
	
	void Emitter::update(float k)
	{
		// first update all particles
		this->_alive = 0;
		foreach_q (Particle*, it, this->particles)
		{
			(*it)->timer += k;
			if (!(*it)->isDead())
			{
				foreach (Affector*, it2, this->affectors)
				{
					(*it2)->update((*it), k);
				}
				if (!(*it)->isDead())
				{
					this->_alive++;
				}
			}
		}
		// remove all expired particles
		while (this->particles.size() > 0)
		{
			if (!this->particles.front()->isDead())
			{
				break;
			}
			delete this->particles.front();
			this->particles.pop_front();
		}
		// create new particles
		this->timer += k;
		if (this->enabled && this->emissionRate > 0.0f)
		{
			this->_cs = 1.0f / this->emissionRate;
			this->_quota = (int)(this->timer * this->emissionRate);
			if (this->_alive >= this->limit)
			{
				this->timer = 0.0f;
			}
			else if (this->timer > this->_cs && this->_alive < this->limit)
			{
				this->_quota = hmin(this->_quota, (int)(this->limit - this->_alive));
				for (int i = 0; i < this->_quota; i++)
				{
					this->_createNewParticle();
				}
				this->timer = 0.0f;
			}
		}
	}
	
	void Emitter::draw(gvec3 point, gvec3 up)
	{
		this->_i = 0;
		foreach_q (Particle*, it, this->particles)
		{
			if (!(*it)->isDead())
			{
				this->_billboard.lookAt((*it)->position, point - (*it)->position, up);
				v[0].set(-(*it)->size.x * (*it)->scale * 0.5f, -(*it)->size.y * (*it)->scale * 0.5f, 0.0f);
				v[1].set((*it)->size.x * (*it)->scale * 0.5f, -(*it)->size.y * (*it)->scale * 0.5f, 0.0f);
				v[2].set(-(*it)->size.x * (*it)->scale * 0.5f, (*it)->size.y * (*it)->scale * 0.5f, 0.0f);
				v[3].set((*it)->size.x * (*it)->scale * 0.5f, (*it)->size.y * (*it)->scale * 0.5f, 0.0f);
			
				this->_rot.setRotation3D(0.0f, 0.0f, 1.0f, (*it)->angle);
				v[0] = this->_rot * v[0];
				v[1] = this->_rot * v[1];
				v[2] = this->_rot * v[2];
				v[3] = this->_rot * v[3];
			
				this->_billboard.inverse();
				v[0] = this->_billboard * v[0];
				v[1] = this->_billboard * v[1];
				v[2] = this->_billboard * v[2];
				v[3] = this->_billboard * v[3];
			
				this->_color = (unsigned int)(*it)->color;
				this->_triangleBatch[this->_i] = v[0];	this->_triangleBatch[this->_i].color = this->_color;		this->_i++;
				this->_triangleBatch[this->_i] = v[1];	this->_triangleBatch[this->_i].color = this->_color;		this->_i++;
				this->_triangleBatch[this->_i] = v[2];	this->_triangleBatch[this->_i].color = this->_color;		this->_i++;
				this->_triangleBatch[this->_i] = v[1];	this->_triangleBatch[this->_i].color = this->_color;		this->_i++;
				this->_triangleBatch[this->_i] = v[2];	this->_triangleBatch[this->_i].color = this->_color;		this->_i++;
				this->_triangleBatch[this->_i] = v[3];	this->_triangleBatch[this->_i].color = this->_color;		this->_i++;
				
			}
		}
		if (this->texture != NULL)
		{
			april::rendersys->setTexture(this->texture);
		}
		april::rendersys->setBlendMode(this->blendMode);
		april::rendersys->render(april::TriangleList, this->_triangleBatch, this->_i);
	}
	
	void Emitter::drawAffectors()
	{
		foreach (Affector*, it, this->affectors)
		{
			(*it)->draw();
		}
	}
	
}

