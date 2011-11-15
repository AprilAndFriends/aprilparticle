/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.3
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

#define RANGE_SEPARATOR ";"
#define RAND_FLOAT_RANGE(name) (this->min ## name < this->max ## name ? hrandf(this->min ## name, this->max ## name) : this->min ## name)
#define RAND_GVEC2_RANGE(name) (this->min ## name != this->max ## name ? this->min ## name + (this->max ## name - this->min ## name) * hrandf(1.0f) : this->min ## name)
#define TRY_SET_TYPE(value, name) if (value == #name) this->setType(name)
#define TRY_GET_TYPE(value, name) if (value == name) return #name;
#define GET_FLOAT_RANGE(name) (this->getMin ## name() != this->getMax ## name() ? \
	hsprintf("%f" RANGE_SEPARATOR "%f", this->getMin ## name(), this->getMax ## name()) : \
	this->getMin ## name())
#define GET_GVEC2_RANGE(name) (this->getMin ## name() != this->getMax ## name() ? \
	hsprintf("%s" RANGE_SEPARATOR "%s", gvec2_to_str(this->getMin ## name()).c_str(), gvec2_to_str(this->getMax ## name()).c_str()) : \
	gvec2_to_str(this->getMin ## name()).c_str())

namespace aprilparticle
{
	gvec3 v[4]; // optimization

	Emitter::Emitter(chstr name) : ActiveObject(name == "" ? generateName("Emitter") : name)
	{
		this->emissionTimer = 0.0f;
		this->time = 0.0f;
		this->type = Point;
		this->dimensions.set(1.0f, 1.0f, 1.0f);
		this->blendMode = april::DEFAULT;
		this->emissionRate = emissionRate;
		this->duration = -1.0f;
		this->delay = 0.0f;
		this->loopDelay = 0.0f;
		this->loops = -1;
		this->currentLoop = 0;
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
		harray<hstr> data = value.split(RANGE_SEPARATOR);
		this->setLifeRange(data.first(), data.last());
	}

	void Emitter::setSize(chstr value)
	{
		harray<hstr> data = value.split(RANGE_SEPARATOR);
		this->setSizeRange(str_to_gvec2(data.first()), str_to_gvec2(data.last()));
	}

	void Emitter::setScale(chstr value)
	{
		harray<hstr> data = value.split(RANGE_SEPARATOR);
		this->setScaleRange(data.first(), data.last());
	}

	void Emitter::setSpeed(chstr value)
	{
		harray<hstr> data = value.split(RANGE_SEPARATOR);
		this->setSpeedRange(data.first(), data.last());
	}

	void Emitter::setAngle(chstr value)
	{
		harray<hstr> data = value.split(APRILPARTICLE_VALUE_SEPARATOR);
		this->setAngleRange(data.first(), data.last());
	}

	bool Emitter::isExpired()
	{
		return (this->loops > 0 && this->currentLoop >= this->loops && this->particles.size() == 0);
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
		if (name == "name")				return this->getName();
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
			return "";
		}
		if (name == "dimensions")		return gvec3_to_str(this->getDimensions());
		if (name == "blend_mode")
		{
			april::BlendMode mode = this->getBlendMode();
			if		(mode == april::DEFAULT)		return "default";
			else if	(mode == april::ALPHA_BLEND)	return "alpha_blend";
			else if	(mode == april::ADD)			return "add";
			else if	(mode == april::SUBTRACT)		return "subtract";
			return "";
		}
		if (name == "emission_rate")	return this->getEmissionRate();
		if (name == "duration")			return this->getDuration();
		if (name == "delay")			return this->getDelay();
		if (name == "loop_delay")		return this->getLoopDelay();
		if (name == "loops")			return this->getLoops();
		if (name == "limit")			return this->getLimit();
		if (name == "life")				return GET_FLOAT_RANGE(Life);
		if (name == "size")				return GET_GVEC2_RANGE(Size);
		if (name == "scale")			return GET_FLOAT_RANGE(Scale);
		if (name == "speed")			return GET_FLOAT_RANGE(Speed);
		if (name == "angle")			return GET_FLOAT_RANGE(Angle);
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
		else if	(name == "duration")		this->setDuration(value);
		else if	(name == "delay")			this->setDelay(value);
		else if	(name == "loop_delay")		this->setLoopDelay(value);
		else if	(name == "loops")			this->setLoops(value);
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
		switch (this->type)
		{
			case Point:
				this->_pos.set(0.0f, 0.0f, 0.0f);
				break;
			case Box:
				this->_pos.x = this->dimensions.x * hrandf(-0.5f, 0.5f);
				this->_pos.y = this->dimensions.y * hrandf(-0.5f, 0.5f);
				this->_pos.z = this->dimensions.z * hrandf(-0.5f, 0.5f);
				break;
			case HollowBox:
				this->_pos.x = this->dimensions.x * 0.5f - this->dimensions.x * hrand(2);
				this->_pos.y = this->dimensions.y * 0.5f - this->dimensions.y * hrand(2);
				this->_pos.z = this->dimensions.z * 0.5f - this->dimensions.z * hrand(2);
				break;
			case Sphere:
				this->_rho = hrandf(1.0f);
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				this->_S = this->_rho * sin(this->_phi);

				this->_pos.x = this->dimensions.x * 0.5f * this->_rho * cos(this->_phi);
				this->_pos.y = this->dimensions.y * 0.5f * this->_S * sin(this->_theta);
				this->_pos.z = this->dimensions.z * 0.5f * this->_S * cos(this->_theta);
				break;
			case HollowSphere:
				this->_rho = 1.0f;
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				this->_S = this->_rho * sin(this->_phi);

				this->_pos.x = this->dimensions.x * 0.5f * this->_rho * cos(this->_phi);
				this->_pos.y = this->dimensions.y * 0.5f * this->_S * sin(this->_theta);
				this->_pos.z = this->dimensions.z * 0.5f * this->_S * cos(this->_theta);
				break;
			case Cylinder:
				this->_rho = hrandf(1.0f);
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				
				this->_pos.x = this->dimensions.x * 0.5f * this->_rho * cos(this->_phi);
				this->_pos.y = this->dimensions.y * hrandf(-0.5f, 0.5f);
				this->_pos.z = this->dimensions.z * 0.5f * this->_rho * sin(this->_phi);
				break;
			case HollowCylinder:
				this->_rho = 1.0f;
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				
				this->_pos.x = this->dimensions.x * 0.5f * this->_rho * cos(this->_phi);
				this->_pos.y = this->dimensions.y * hrandf(-0.5f, 0.5f);
				this->_pos.z = this->dimensions.z * 0.5f * this->_rho * sin(this->_phi);
				break;
		}
		this->_pos += this->position;
		if (this->system != NULL)
		{
			this->_pos += this->system->getPosition();
		}
		this->_particle = new Particle(this->_pos, RAND_FLOAT_RANGE(Life), RAND_GVEC2_RANGE(Size),
			RAND_FLOAT_RANGE(Scale), RAND_FLOAT_RANGE(Speed), RAND_FLOAT_RANGE(Angle));
		this->particles += this->_particle;
		foreach (Affector*, it, this->affectors)
		{
			(*it)->update(this->_particle, 0.0f, this->_movement);
		}
	}
	
	void Emitter::update(float k)
	{
		if (!this->enabled)
		{
			return;
		}
		// check delay
		this->time += k;
		if (this->delay > 0.0f)
		{
			if (this->time <= this->delay)
			{
				return;
			}
			this->time -= this->delay;
			this->delay = 0.0f;
			k = hmin(k, this->time);
		}
		// check duration and looping setup
		this->_expired = false;
		if (this->duration > 0.0f && this->time >= this->duration)
		{
			if (this->loopDelay > 0.0f)
			{
				if (this->time >= this->duration + this->loopDelay)
				{
					while (this->time >= this->duration + this->loopDelay)
					{
						this->time -= this->duration + this->loopDelay;
						this->currentLoop++;
					}
				}
				if (this->time >= this->duration)
				{
					this->_expired = true;
				}
			}
			else
			{
				while (this->time >= this->duration)
				{
					this->time -= this->duration;
					this->currentLoop++;
				}
			}
		}
		// first update all particles
		this->_alive = 0;
		foreach_q (Particle*, it, this->particles)
		{
			(*it)->timer += k;
			if (!(*it)->isDead())
			{
				this->_movement.set(0.0f, 0.0f, 0.0f);
				foreach (Affector*, it2, this->affectors)
				{
					(*it2)->update((*it), k, this->_movement);
				}
				(*it)->position += this->_movement;
				this->_alive++;
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
		// check repetition loops
		if (this->_expired || this->loops > 0 && this->currentLoop >= this->loops)
		{
			return;
		}
		this->emissionTimer += k;
		// create new particles
		if (this->emissionRate > 0.0f)
		{
			this->_cs = 1.0f / this->emissionRate;
			this->_quota = (int)(this->emissionTimer * this->emissionRate);
			if (this->_alive >= this->limit)
			{
				this->emissionTimer = 0.0f;
			}
			else if (this->emissionTimer > this->_cs && this->_alive < this->limit)
			{
				this->_quota = hmin(this->_quota, (int)(this->limit - this->_alive));
				for (int i = 0; i < this->_quota; i++)
				{
					this->_createNewParticle();
				}
				this->emissionTimer = 0.0f;
			}
		}
	}
	
	void Emitter::draw(gvec3 point, gvec3 up)
	{
		if (!this->visible)
		{
			return;
		}
		this->_i = 0;
		foreach_q (Particle*, it, this->particles)
		{
			if (!(*it)->isDead() && (*it)->color.a > 0)
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
		if (this->_i > 0)
		{
			april::rendersys->render(april::TriangleList, this->_triangleBatch, this->_i);
		}
	}
	
	void Emitter::draw2D()
	{
		this->_i = 0;
		this->_w = (float)this->texture->getWidth();
		this->_h = (float)this->texture->getHeight();
		foreach_q (Particle*, it, this->particles)
		{
			if (!(*it)->isDead())
			{
				v[0].set((*it)->position.x - (*it)->size.x * this->_w * (*it)->scale * 0.5f, (*it)->position.y - (*it)->size.y * this->_h * (*it)->scale * 0.5f, 0.0f);
				v[1].set((*it)->position.x + (*it)->size.x * this->_w * (*it)->scale * 0.5f, (*it)->position.y - (*it)->size.y * this->_h * (*it)->scale * 0.5f, 0.0f);
				v[2].set((*it)->position.x - (*it)->size.x * this->_w * (*it)->scale * 0.5f, (*it)->position.y + (*it)->size.y * this->_h * (*it)->scale * 0.5f, 0.0f);
				v[3].set((*it)->position.x + (*it)->size.x * this->_w * (*it)->scale * 0.5f, (*it)->position.y + (*it)->size.y * this->_h * (*it)->scale * 0.5f, 0.0f);
				
				this->_rot.setRotation3D(0.0f, 0.0f, 1.0f, (*it)->angle);
				v[0] = this->_rot * v[0];
				v[1] = this->_rot * v[1];
				v[2] = this->_rot * v[2];
				v[3] = this->_rot * v[3];
				
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
		if (this->_i > 0)
		{
			april::rendersys->render(april::TriangleList, this->_triangleBatch, this->_i);
		}
	}
	
	void Emitter::drawAffectors()
	{
		foreach (Affector*, it, this->affectors)
		{
			(*it)->draw();
		}
	}
	
}

