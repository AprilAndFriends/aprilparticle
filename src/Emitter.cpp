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

#include <april/Color.h>
#include <gtypes/Constants.h>
#include <gtypes/Matrix3.h>
#include <gtypes/Matrix4.h>
#include <gtypes/Vector2.h>
#include <hltypes/hdeque.h>
#include <hltypes/hltypesUtil.h>

#include "Affector.h"
#include "aprilparticle.h"
#include "aprilparticleUtil.h"
#include "Emitter.h"
#include "Particle.h"
#include "System.h"

namespace aprilparticle
{
	static gvec3 v[4]; // optimization

	Emitter::Emitter(chstr name) : ActiveObject(name == "" ? generateName("Emitter") : name)
	{
		this->emissionTimer = 0.0f;
		this->loopTimer = 0.0f;
		this->time = 0.0f;
		this->running = true;
		this->type = Point;
		this->dimensions.set(1.0f, 1.0f, 1.0f);
		this->blendMode = april::DEFAULT;
		this->emissionRate = emissionRate;
		this->duration = -1.0f;
		this->delay = 0.0f;
		this->loopDelay = 0.0f;
		this->loops = -1;
		this->currentLoop = 0;
		this->alive = 0;
		this->limit = 10;
		this->preUpdate = 0.0f;
		this->minLife = 1.0f;
		this->maxLife = 1.0f;
		this->minDirection.set(0.0f, 0.0f, 0.0f);
		this->maxDirection.set(0.0f, 0.0f, 0.0f);
		this->minSize.set(1.0f, 1.0f);
		this->maxSize.set(1.0f, 1.0f);
		this->minScale = 1.0f;
		this->maxScale = 1.0f;
		this->minAngle = 0.0f;
		this->maxAngle = 0.0f;
		this->texture = NULL;
		this->_triangleBatch = NULL;
		this->_setupTriangleBatch();
		this->system = NULL;
		this->started = false;
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
		for_iterx (this->_i, 0, this->limit)
		{
			this->_triangleBatch[this->_i * 6 + 0].u = 0.0f;		this->_triangleBatch[this->_i * 6 + 0].v = 0.0f;
			this->_triangleBatch[this->_i * 6 + 1].u = 1.0f;		this->_triangleBatch[this->_i * 6 + 1].v = 0.0f;
			this->_triangleBatch[this->_i * 6 + 2].u = 0.0f;		this->_triangleBatch[this->_i * 6 + 2].v = 1.0f;
			this->_triangleBatch[this->_i * 6 + 3].u = 1.0f;		this->_triangleBatch[this->_i * 6 + 3].v = 0.0f;
			this->_triangleBatch[this->_i * 6 + 4].u = 0.0f;		this->_triangleBatch[this->_i * 6 + 4].v = 1.0f;
			this->_triangleBatch[this->_i * 6 + 5].u = 1.0f;		this->_triangleBatch[this->_i * 6 + 5].v = 1.0f;
		}
	}

	void Emitter::setLife(float value)
	{
		this->minLife = value;
		this->maxLife = value;
	}

	void Emitter::setDirection(gvec3 value)
	{
		this->minDirection = value;
		this->maxDirection = value;
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

	void Emitter::setAngle(float value)
	{
		this->minAngle = value;
		this->maxAngle = value;
	}

	void Emitter::setLife(chstr value)
	{
		harray<hstr> data = value.split(APRILPARTICLE_RANGE_SEPARATOR);
		this->setLifeRange(data.first(), data.last());
	}

	void Emitter::setDirection(chstr value)
	{
		harray<hstr> data = value.split(APRILPARTICLE_RANGE_SEPARATOR);
		this->setDirectionRange(hstr_to_gvec3(data.first()), hstr_to_gvec3(data.last()));
	}

	void Emitter::setSize(chstr value)
	{
		harray<hstr> data = value.split(APRILPARTICLE_RANGE_SEPARATOR);
		this->setSizeRange(hstr_to_gvec2(data.first()), hstr_to_gvec2(data.last()));
	}

	void Emitter::setScale(chstr value)
	{
		harray<hstr> data = value.split(APRILPARTICLE_RANGE_SEPARATOR);
		this->setScaleRange(data.first(), data.last());
	}

	void Emitter::setAngle(chstr value)
	{
		harray<hstr> data = value.split(APRILPARTICLE_RANGE_SEPARATOR);
		this->setAngleRange(data.first(), data.last());
	}

	int Emitter::getParticleCount()
	{
		return this->alive;
	}

	bool Emitter::isExpired()
	{
		return (!this->running && this->particles.size() == 0);
	}

	void Emitter::setLifeRange(float min, float max)
	{
		this->minLife = min;
		this->maxLife = max;
	}

	void Emitter::setDirectionRange(gvec3 min, gvec3 max)
	{
		this->minDirection = min;
		this->maxDirection = max;
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
		if (name == "dimensions")		return gvec3_to_hstr(this->getDimensions());
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
		if (name == "pre_update")		return this->getPreUpdate();
		if (name == "life")				return GET_RANGE(Life, hstr);
		if (name == "direction")		return GET_RANGE(Direction, gvec3_to_hstr);
		if (name == "size")				return GET_RANGE(Size, gvec2_to_hstr);
		if (name == "scale")			return GET_RANGE(Scale, hstr);
		if (name == "angle")			return GET_RANGE(Angle, hstr);
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
		else if	(name == "dimensions")		this->setDimensions(hstr_to_gvec3(value));
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
		else if	(name == "pre_update")		this->setPreUpdate(value);
		else if	(name == "life")			this->setLife(value);
		else if	(name == "direction")		this->setDirection(value);
		else if	(name == "size")			this->setSize(value);
		else if	(name == "scale")			this->setScale(value);
		else if	(name == "angle")			this->setAngle(value);
		else return ActiveObject::setProperty(name, value);
		return true;
	}

	void Emitter::_createNewParticle(float k)
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
				this->_angle = this->_rho * sin(this->_phi);

				this->_pos.x = this->dimensions.x * 0.5f * this->_rho * cos(this->_phi);
				this->_pos.y = this->dimensions.y * 0.5f * this->_angle * sin(this->_theta);
				this->_pos.z = this->dimensions.z * 0.5f * this->_angle * cos(this->_theta);
				break;
			case HollowSphere:
				this->_rho = 1.0f;
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				this->_angle = this->_rho * sin(this->_phi);

				this->_pos.x = this->dimensions.x * 0.5f * this->_rho * cos(this->_phi);
				this->_pos.y = this->dimensions.y * 0.5f * this->_angle * sin(this->_theta);
				this->_pos.z = this->dimensions.z * 0.5f * this->_angle * cos(this->_theta);
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
		this->_particle = new Particle();
		this->_particle->life = RAND_RANGE(Life);
		this->_particle->position = this->_pos;
		this->_particle->direction = RAND_RANGE(Direction);
		this->_particle->size = RAND_RANGE(Size);
		this->_particle->scale = RAND_RANGE(Scale);
		this->_particle->angle = RAND_RANGE(Angle);
		this->_movement.set(0.0f, 0.0f, 0.0f);
		foreach (Affector*, it, this->affectors)
		{
			(*it)->update(this->_particle, k, this->_movement);
		}
		this->_particle->position += this->_movement + this->_particle->direction * k;
		this->particles += this->_particle;
	}

	void Emitter::reset()
	{
		this->emissionTimer = 0.0f;
		this->loopTimer = 0.0f;
		this->time = 0.0f;
		this->currentLoop = 0;
		this->running = true;
		foreach_q (Particle*, it, this->particles)
		{
			delete (*it);
		}
		this->particles.clear();
	}
	
	void Emitter::update(float k)
	{
		if (!this->enabled)
		{
			return;
		}
		if (!this->started)
		{
			this->started = true;
			if (this->preUpdate > 0.0f)
			{
				int count = (int)(this->preUpdate / 0.1f);
				for_iter (i, 0, count)
				{
					this->update(0.1f);
				}
			}
		}
		// check delay
		this->time += k;
		this->loopTimer += k;
		if (this->delay > 0.0f)
		{
			if (this->time <= this->delay)
			{
				return;
			}
			if (this->time - k < this->delay)
			{
				this->loopTimer -= this->delay;
				k = hmin(k, this->loopTimer);
			}
		}
		// check duration and looping setup
		if (this->running)
		{
			this->_expired = false;
			if (this->duration > 0.0f && this->loopTimer >= this->duration)
			{
				if (this->loopDelay > 0.0f)
				{
					if (this->loopTimer >= this->duration + this->loopDelay)
					{
						while (this->loopTimer >= this->duration + this->loopDelay)
						{
							this->loopTimer -= this->duration + this->loopDelay;
							this->currentLoop++;
						}
					}
					if (this->loopTimer >= this->duration)
					{
						this->_expired = true;
					}
				}
				else
				{
					while (this->loopTimer >= this->duration)
					{
						this->loopTimer -= this->duration;
						this->currentLoop++;
					}
				}
			}
		}
		// first update all particles
		this->alive = 0;
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
				(*it)->position += this->_movement + (*it)->direction * k;
				this->alive++;
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
		if (this->loops > 0 && this->currentLoop >= this->loops)
		{
			this->running = false;
		}
		if (this->running && !this->_expired)
		{
			this->emissionTimer += k;
			// create new particles
			if (this->emissionRate > 0.0f)
			{
				this->_cs = 1.0f / this->emissionRate;
				this->_quota = (int)(this->emissionTimer * this->emissionRate);
				if (this->alive >= this->limit)
				{
					this->emissionTimer = 0.0f;
				}
				else if (this->emissionTimer > this->_cs && this->alive < this->limit)
				{
					this->_quota = hmin(this->_quota, (int)(this->limit - this->alive));
					for_iter (i, 0, this->_quota)
					{
						this->_createNewParticle(k * i / this->_quota);
					}
					this->emissionTimer = 0.0f;
				}
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
				this->_xSize = (*it)->size.x * (*it)->scale * 0.5f;
				this->_ySize = (*it)->size.y * (*it)->scale * 0.5f;
				v[0].set(-this->_xSize, -this->_ySize, 0.0f);
				v[1].set(this->_xSize, -this->_ySize, 0.0f);
				v[2].set(-this->_xSize, this->_ySize, 0.0f);
				v[3].set(this->_xSize, this->_ySize, 0.0f);
				
				this->_rot.setRotation3D(0.0f, 0.0f, -1.0f, (*it)->angle);
				v[0] = this->_rot * v[0];
				v[1] = this->_rot * v[1];
				v[2] = this->_rot * v[2];
				v[3] = this->_rot * v[3];
			
				this->_billboard.lookAt((*it)->position, point - (*it)->position, -up);
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
		if (this->_i > 0)
		{
			april::rendersys->setTexture(this->texture);
			april::rendersys->setBlendMode(this->blendMode);
			april::rendersys->render(april::TriangleList, this->_triangleBatch, this->_i);
		}
	}
	
	void Emitter::draw(gvec2 offset)
	{
		this->_i = 0;
		this->_w = (float)this->texture->getWidth();
		this->_h = (float)this->texture->getHeight();
		foreach_q (Particle*, it, this->particles)
		{
			if (!(*it)->isDead())
			{
				this->_xSize = (*it)->size.x * this->_w * (*it)->scale * 0.5f;
				this->_ySize = (*it)->size.y * this->_h * (*it)->scale * 0.5f;
				v[0].set(-this->_xSize, -this->_ySize, 0.0f);
				v[1].set(this->_xSize, -this->_ySize, 0.0f);
				v[2].set(-this->_xSize, this->_ySize, 0.0f);
				v[3].set(this->_xSize, this->_ySize, 0.0f);

				this->_offset.set((*it)->position.x + offset.x, (*it)->position.y + offset.y, 0.0f);
				if ((*it)->angle != 0.0f)
				{
					this->_rot.setRotation3D(0.0f, 0.0f, -1.0f, (*it)->angle);
					v[0] = this->_rot * v[0] + this->_offset;
					v[1] = this->_rot * v[1] + this->_offset;
					v[2] = this->_rot * v[2] + this->_offset;
					v[3] = this->_rot * v[3] + this->_offset;
				}
				else
				{
					v[0] += this->_offset;
					v[1] += this->_offset;
					v[2] += this->_offset;
					v[3] += this->_offset;
				}
				
				this->_color = (unsigned int)(*it)->color;
				this->_triangleBatch[this->_i] = v[0];	this->_triangleBatch[this->_i].color = this->_color;		this->_i++;
				this->_triangleBatch[this->_i] = v[1];	this->_triangleBatch[this->_i].color = this->_color;		this->_i++;
				this->_triangleBatch[this->_i] = v[2];	this->_triangleBatch[this->_i].color = this->_color;		this->_i++;
				this->_triangleBatch[this->_i] = v[1];	this->_triangleBatch[this->_i].color = this->_color;		this->_i++;
				this->_triangleBatch[this->_i] = v[2];	this->_triangleBatch[this->_i].color = this->_color;		this->_i++;
				this->_triangleBatch[this->_i] = v[3];	this->_triangleBatch[this->_i].color = this->_color;		this->_i++;
			}
		}
		if (this->_i > 0)
		{
			april::rendersys->setTexture(this->texture);
			april::rendersys->setBlendMode(this->blendMode);
			april::rendersys->render(april::TriangleList, this->_triangleBatch, this->_i);
		}
	}
	
	void Emitter::draw(gvec2 offset, april::Color color)
	{
		this->_i = 0;
		this->_w = (float)this->texture->getWidth();
		this->_h = (float)this->texture->getHeight();
		foreach_q (Particle*, it, this->particles)
		{
			if (!(*it)->isDead())
			{
				this->_xSize = (*it)->size.x * this->_w * (*it)->scale * 0.5f;
				this->_ySize = (*it)->size.y * this->_h * (*it)->scale * 0.5f;
				v[0].set(-this->_xSize, -this->_ySize, 0.0f);
				v[1].set(this->_xSize, -this->_ySize, 0.0f);
				v[2].set(-this->_xSize, this->_ySize, 0.0f);
				v[3].set(this->_xSize, this->_ySize, 0.0f);

				this->_offset.set((*it)->position.x + offset.x, (*it)->position.y + offset.y, 0.0f);
				if ((*it)->angle != 0.0f)
				{
					this->_rot.setRotation3D(0.0f, 0.0f, 1.0f, (*it)->angle);
					v[0] = this->_rot * v[0] + this->_offset;
					v[1] = this->_rot * v[1] + this->_offset;
					v[2] = this->_rot * v[2] + this->_offset;
					v[3] = this->_rot * v[3] + this->_offset;
				}
				else
				{
					v[0] += this->_offset;
					v[1] += this->_offset;
					v[2] += this->_offset;
					v[3] += this->_offset;
				}
				
				this->_color = (unsigned int)((*it)->color * color);
				this->_triangleBatch[this->_i] = v[0];	this->_triangleBatch[this->_i].color = this->_color;		this->_i++;
				this->_triangleBatch[this->_i] = v[1];	this->_triangleBatch[this->_i].color = this->_color;		this->_i++;
				this->_triangleBatch[this->_i] = v[2];	this->_triangleBatch[this->_i].color = this->_color;		this->_i++;
				this->_triangleBatch[this->_i] = v[1];	this->_triangleBatch[this->_i].color = this->_color;		this->_i++;
				this->_triangleBatch[this->_i] = v[2];	this->_triangleBatch[this->_i].color = this->_color;		this->_i++;
				this->_triangleBatch[this->_i] = v[3];	this->_triangleBatch[this->_i].color = this->_color;		this->_i++;
			}
		}
		if (this->_i > 0)
		{
			april::rendersys->setTexture(this->texture);
			april::rendersys->setBlendMode(this->blendMode);
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

