/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 2.04
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
#include <hltypes/hlog.h>
#include <hltypes/hltypesUtil.h>

#include "Affector.h"
#include "aprilparticle.h"
#include "aprilparticleUtil.h"
#include "Emitter.h"
#include "Particle.h"
#include "System.h"
#include "Space.h"

namespace aprilparticle
{
	static gvec3 v[4]; // optimization

	Emitter::Emitter(chstr name) : SpaceObject(name == "" ? generateName("Emitter") : name)
	{
		this->emissionTimer = 0.0f;
		this->loopTimer = 0.0f;
		this->time = 0.0f;
		this->running = true;
		this->type = Point;
		this->dimensions.set(1.0f, 1.0f, 1.0f);
		this->blendMode = april::BM_DEFAULT;
		this->colorMode = april::CM_DEFAULT;
		this->colorModeFactor = 1.0f;
		this->emissionRate = 0.0f;
		this->duration = -1.0f;
		this->delay = 0.0f;
		this->loopDelay = 0.0f;
		this->loops = -1;
		this->currentLoop = 0;
		this->alive = 0;
		this->limit = 10;
		this->reverseRendering = false;
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
		this->space = NULL;
	}

	Emitter::Emitter(const Emitter& other) : SpaceObject(other)
	{
		this->emissionTimer = other.emissionTimer;
		this->loopTimer = other.loopTimer;
		this->time = other.time;
		this->running = other.running;
		this->type = other.type;
		this->dimensions = other.dimensions;
		this->blendMode = other.blendMode;
		this->colorMode = other.colorMode;
		this->colorModeFactor = other.colorModeFactor;
		this->emissionRate = other.emissionRate;
		this->duration = other.duration;
		this->delay = other.delay;
		this->loopDelay = other.loopDelay;
		this->loops = other.loops;
		this->currentLoop = other.currentLoop;
		this->alive = other.alive;
		this->limit = other.limit;
		this->reverseRendering = other.reverseRendering;
		this->minLife = other.minLife;
		this->maxLife = other.maxLife;
		this->minDirection = other.minDirection;
		this->maxDirection = other.maxDirection;
		this->minSize = other.minSize;
		this->maxSize = other.maxSize;
		this->minScale = other.minScale;
		this->maxScale = other.maxScale;
		this->minAngle = other.minAngle;
		this->maxAngle = other.maxAngle;
		this->texture = NULL;
		this->_triangleBatch = NULL;
		this->_setupTriangleBatch();
		this->space = NULL;
	}

	Emitter::~Emitter()
	{
		if (this->_triangleBatch != NULL)
		{
			delete [] this->_triangleBatch;
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
		harray<hstr> data = value.split(aprilparticle::SeparatorRange);
		this->setLifeRange(data.first(), data.last());
	}

	void Emitter::setDirection(chstr value)
	{
		harray<hstr> data = value.split(aprilparticle::SeparatorRange);
		this->setDirectionRange(hstr_to_gvec3(data.first()), hstr_to_gvec3(data.last()));
	}

	void Emitter::setSize(chstr value)
	{
		harray<hstr> data = value.split(aprilparticle::SeparatorRange);
		this->setSizeRange(hstr_to_gvec2(data.first()), hstr_to_gvec2(data.last()));
	}

	void Emitter::setScale(chstr value)
	{
		harray<hstr> data = value.split(aprilparticle::SeparatorRange);
		this->setScaleRange(data.first(), data.last());
	}

	void Emitter::setAngle(chstr value)
	{
		harray<hstr> data = value.split(aprilparticle::SeparatorRange);
		this->setAngleRange(data.first(), data.last());
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

	hstr Emitter::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "name")					return this->getName();
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
		if (name == "dimensions")			return gvec3_to_hstr(this->getDimensions());
		if (name == "blend_mode")
		{
			april::BlendMode mode = this->getBlendMode();
			if		(mode == april::BM_DEFAULT)		return "default";
			else if	(mode == april::BM_ALPHA)		return "alpha";
			else if	(mode == april::BM_ADD)			return "add";
			else if	(mode == april::BM_SUBTRACT)	return "subtract";
			else if	(mode == april::BM_OVERWRITE)	return "overwrite";
			return "";
		}
		if (name == "color_mode")
		{
			april::ColorMode mode = this->getColorMode();
			if		(mode == april::CM_DEFAULT)		return "default";
			else if	(mode == april::CM_MULTIPLY)	return "multiply";
			else if	(mode == april::CM_LERP)		return "lerp";
			else if	(mode == april::CM_ALPHA_MAP)	return "alpha_map";
			return "";
		}
		if (name == "color_mode_factor")	return this->getColorModeFactor();
		if (name == "emission_rate")		return this->getEmissionRate();
		if (name == "duration")				return this->getDuration();
		if (name == "delay")				return this->getDelay();
		if (name == "loop_delay")			return this->getLoopDelay();
		if (name == "loops")				return this->getLoops();
		if (name == "limit")				return this->getLimit();
		if (name == "reverse_rendering")	return this->isReverseRendering();
		if (name == "life")					return GET_RANGE(Life, hstr);
		if (name == "direction")			return GET_RANGE(Direction, gvec3_to_hstr);
		if (name == "size")					return GET_RANGE(Size, gvec2_to_hstr);
		if (name == "scale")				return GET_RANGE(Scale, hstr);
		if (name == "angle")				return GET_RANGE(Angle, hstr);
		return SpaceObject::getProperty(name, property_exists);
	}

	bool Emitter::setProperty(chstr name, chstr value)
	{
		if		(name == "name")				this->setName(value);
		else if	(name == "type")
		{
			TRY_SET_TYPE(value, Point);
			else TRY_SET_TYPE(value, Box);
			else TRY_SET_TYPE(value, HollowBox);
			else TRY_SET_TYPE(value, Sphere);
			else TRY_SET_TYPE(value, HollowSphere);
			else TRY_SET_TYPE(value, Cylinder);
			else TRY_SET_TYPE(value, HollowCylinder);
			else hlog::warnf(aprilparticle::logTag, "Value '%s' does not exist for property '%s' in '%s'!", value.c_str(), name.c_str(), this->name.c_str());
		}
		else if	(name == "dimensions")			this->setDimensions(hstr_to_gvec3(value));
		else if	(name == "blend_mode")
		{
			if		(value == "default")		this->setBlendMode(april::BM_DEFAULT);
			else if	(value == "alpha")			this->setBlendMode(april::BM_ALPHA);
			else if	(value == "add")			this->setBlendMode(april::BM_ADD);
			else if	(value == "subtract")		this->setBlendMode(april::BM_SUBTRACT);
			else if	(value == "overwrite")		this->setBlendMode(april::BM_OVERWRITE);
			else hlog::warnf(aprilparticle::logTag, "Value '%s' does not exist for property '%s' in '%s'!", value.c_str(), name.c_str(), this->name.c_str());
		}
		else if	(name == "color_mode")
		{
			if		(value == "default")		this->setColorMode(april::CM_DEFAULT);
			else if	(value == "multiply")		this->setColorMode(april::CM_MULTIPLY);
			else if	(value == "lerp")			this->setColorMode(april::CM_LERP);
			else if	(value == "alpha_map")		this->setColorMode(april::CM_ALPHA_MAP);
			else hlog::warnf(aprilparticle::logTag, "Value '%s' does not exist for property '%s' in '%s'!", value.c_str(), name.c_str(), this->name.c_str());
		}
		else if	(name == "color_mode_factor")	this->setColorModeFactor(value);
		else if	(name == "emission_rate")		this->setEmissionRate(value);
		else if	(name == "limit")				this->setLimit(value);
		else if	(name == "duration")			this->setDuration(value);
		else if	(name == "delay")				this->setDelay(value);
		else if	(name == "loop_delay")			this->setLoopDelay(value);
		else if	(name == "loops")				this->setLoops(value);
		else if (name == "reverse_rendering")	this->setReverseRendering(value);
		else if	(name == "life")				this->setLife(value);
		else if	(name == "direction")			this->setDirection(value);
		else if	(name == "size")				this->setSize(value);
		else if	(name == "scale")				this->setScale(value);
		else if	(name == "angle")				this->setAngle(value);
		else return SpaceObject::setProperty(name, value);
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
		if (this->space != NULL)
		{
			this->_pos += this->space->getPosition();
		}
		this->space->_particle = new Particle();
		this->space->_particle->life = RAND_RANGE(Life);
		this->space->_particle->position = this->_pos;
		this->space->_particle->direction = RAND_RANGE(Direction);
		this->space->_particle->size = RAND_RANGE(Size);
		this->space->_particle->scale = RAND_RANGE(Scale);
		this->space->_particle->angle = RAND_RANGE(Angle);
		this->particles += this->space->_particle;
		this->space->_addNewParticle(k);
	}

	void Emitter::reset()
	{
		this->emissionTimer = 0.0f;
		this->loopTimer = 0.0f;
		this->time = 0.0f;
		this->currentLoop = 0;
		this->running = true;
		this->alive = 0;
		this->particles.clear();
	}

	void Emitter::update(float k)
	{
		// remove all expired particles
		if (this->particles.size() > 0)
		{
			int i = 0;
			for_iterx (i, 0, this->particles.size())
			{
				if (!this->particles[i]->isDead())
				{
					break;
				}
				// don't delete any particles, Space takes care of that
			}
			if (i > 0)
			{
				this->particles.remove_first(i);
			}
		}
		// count alive particles
		this->alive = 0;
		foreach_q (Particle*, it, this->particles)
		{
			if (!(*it)->isDead())
			{
				++this->alive;
			}
		}
		if (!this->enabled)
		{
			return;
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
							++this->currentLoop;
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
						++this->currentLoop;
					}
				}
			}
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
					this->alive += this->_quota;
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
		this->_pStart = 0;
		this->_pEnd = this->particles.size();
		this->_pStep = 1;
		if (this->reverseRendering)
		{
			this->_pStart = this->particles.size() - 1;
			this->_pEnd = -1;
			this->_pStep = -1;
		}
		this->_i = 0;
		for (this->_pI = this->_pStart; this->_pI != this->_pEnd; this->_pI += this->_pStep)
		{
			this->space->_particle = this->particles[this->_pI];
			if (!this->space->_particle->isDead() && this->space->_particle->color.a > 0)
			{
				this->_xSize = this->space->_particle->size.x * this->space->_particle->scale * 0.5f;
				this->_ySize = this->space->_particle->size.y * this->space->_particle->scale * 0.5f;
				v[0].set(-this->_xSize, -this->_ySize, 0.0f);
				v[1].set(this->_xSize, -this->_ySize, 0.0f);
				v[2].set(-this->_xSize, this->_ySize, 0.0f);
				v[3].set(this->_xSize, this->_ySize, 0.0f);
				
				this->_rot.setRotation3D(0.0f, 0.0f, -1.0f, this->space->_particle->angle);
				v[0] = this->_rot * v[0];
				v[1] = this->_rot * v[1];
				v[2] = this->_rot * v[2];
				v[3] = this->_rot * v[3];
			
				this->_billboard.lookAt(this->space->_particle->position, point - this->space->_particle->position, -up);
				this->_billboard.inverse();
				v[0] = this->_billboard * v[0];
				v[1] = this->_billboard * v[1];
				v[2] = this->_billboard * v[2];
				v[3] = this->_billboard * v[3];
			
				this->_color = (unsigned int)this->space->_particle->color;
				this->_triangleBatch[this->_i] = v[0];	this->_triangleBatch[this->_i].color = this->_color;		++this->_i;
				this->_triangleBatch[this->_i] = v[1];	this->_triangleBatch[this->_i].color = this->_color;		++this->_i;
				this->_triangleBatch[this->_i] = v[2];	this->_triangleBatch[this->_i].color = this->_color;		++this->_i;
				this->_triangleBatch[this->_i] = v[1];	this->_triangleBatch[this->_i].color = this->_color;		++this->_i;
				this->_triangleBatch[this->_i] = v[2];	this->_triangleBatch[this->_i].color = this->_color;		++this->_i;
				this->_triangleBatch[this->_i] = v[3];	this->_triangleBatch[this->_i].color = this->_color;		++this->_i;
			}
		}
		if (this->_i > 0)
		{
			april::rendersys->setTexture(this->texture);
			april::rendersys->setTextureBlendMode(this->blendMode);
			april::rendersys->setTextureColorMode(this->colorMode, this->colorModeFactor);
			april::rendersys->render(april::RO_TRIANGLE_LIST, this->_triangleBatch, this->_i);
		}
	}
	
	void Emitter::draw(gvec2 offset)
	{
		this->texture->load();
		this->_w = (float)this->texture->getWidth();
		this->_h = (float)this->texture->getHeight();
		this->_pStart = 0;
		this->_pEnd = this->particles.size();
		this->_pStep = 1;
		if (this->reverseRendering)
		{
			this->_pStart = this->particles.size() - 1;
			this->_pEnd = -1;
			this->_pStep = -1;
		}
		this->_i = 0;
		for (this->_pI = this->_pStart; this->_pI != this->_pEnd; this->_pI += this->_pStep)
		{
			this->space->_particle = this->particles[this->_pI];
			if (!this->space->_particle->isDead())
			{
				this->_xSize = this->space->_particle->size.x * this->_w * this->space->_particle->scale * 0.5f;
				this->_ySize = this->space->_particle->size.y * this->_h * this->space->_particle->scale * 0.5f;
				v[0].set(-this->_xSize, -this->_ySize, 0.0f);
				v[1].set(this->_xSize, -this->_ySize, 0.0f);
				v[2].set(-this->_xSize, this->_ySize, 0.0f);
				v[3].set(this->_xSize, this->_ySize, 0.0f);

				this->_offset.set(this->space->_particle->position.x + offset.x, this->space->_particle->position.y + offset.y, 0.0f);
				if (this->space->_particle->angle != 0.0f)
				{
					this->_rot.setRotation3D(0.0f, 0.0f, -1.0f, this->space->_particle->angle);
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
				
				this->_color = (unsigned int)this->space->_particle->color;
				this->_triangleBatch[this->_i] = v[0];	this->_triangleBatch[this->_i].color = this->_color;		++this->_i;
				this->_triangleBatch[this->_i] = v[1];	this->_triangleBatch[this->_i].color = this->_color;		++this->_i;
				this->_triangleBatch[this->_i] = v[2];	this->_triangleBatch[this->_i].color = this->_color;		++this->_i;
				this->_triangleBatch[this->_i] = v[1];	this->_triangleBatch[this->_i].color = this->_color;		++this->_i;
				this->_triangleBatch[this->_i] = v[2];	this->_triangleBatch[this->_i].color = this->_color;		++this->_i;
				this->_triangleBatch[this->_i] = v[3];	this->_triangleBatch[this->_i].color = this->_color;		++this->_i;
			}
		}
		if (this->_i > 0)
		{
			april::rendersys->setTexture(this->texture);
			april::rendersys->setTextureBlendMode(this->blendMode);
			april::rendersys->setTextureColorMode(this->colorMode, this->colorModeFactor);
			april::rendersys->render(april::RO_TRIANGLE_LIST, this->_triangleBatch, this->_i);
		}
	}
	
	void Emitter::draw(gvec2 offset, april::Color color)
	{
		this->texture->load();
		this->_w = (float)this->texture->getWidth();
		this->_h = (float)this->texture->getHeight();
		this->_pStart = 0;
		this->_pEnd = this->particles.size();
		this->_pStep = 1;
		if (this->reverseRendering)
		{
			this->_pStart = this->particles.size() - 1;
			this->_pEnd = -1;
			this->_pStep = -1;
		}
		this->_i = 0;
		for (this->_pI = this->_pStart; this->_pI != this->_pEnd; this->_pI += this->_pStep)
		{
			this->space->_particle = this->particles[this->_pI];
			if (!this->space->_particle->isDead())
			{
				this->_xSize = this->space->_particle->size.x * this->_w * this->space->_particle->scale * 0.5f;
				this->_ySize = this->space->_particle->size.y * this->_h * this->space->_particle->scale * 0.5f;
				v[0].set(-this->_xSize, -this->_ySize, 0.0f);
				v[1].set(this->_xSize, -this->_ySize, 0.0f);
				v[2].set(-this->_xSize, this->_ySize, 0.0f);
				v[3].set(this->_xSize, this->_ySize, 0.0f);

				this->_offset.set(this->space->_particle->position.x + offset.x, this->space->_particle->position.y + offset.y, 0.0f);
				if (this->space->_particle->angle != 0.0f)
				{
					this->_rot.setRotation3D(0.0f, 0.0f, -1.0f, this->space->_particle->angle);
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
				
				this->_color = (unsigned int)(this->space->_particle->color * color);
				this->_triangleBatch[this->_i] = v[0];	this->_triangleBatch[this->_i].color = this->_color;		++this->_i;
				this->_triangleBatch[this->_i] = v[1];	this->_triangleBatch[this->_i].color = this->_color;		++this->_i;
				this->_triangleBatch[this->_i] = v[2];	this->_triangleBatch[this->_i].color = this->_color;		++this->_i;
				this->_triangleBatch[this->_i] = v[1];	this->_triangleBatch[this->_i].color = this->_color;		++this->_i;
				this->_triangleBatch[this->_i] = v[2];	this->_triangleBatch[this->_i].color = this->_color;		++this->_i;
				this->_triangleBatch[this->_i] = v[3];	this->_triangleBatch[this->_i].color = this->_color;		++this->_i;
			}
		}
		if (this->_i > 0)
		{
			april::rendersys->setTexture(this->texture);
			april::rendersys->setTextureBlendMode(this->blendMode);
			april::rendersys->setTextureColorMode(this->colorMode, this->colorModeFactor);
			april::rendersys->render(april::RO_TRIANGLE_LIST, this->_triangleBatch, this->_i);
		}
	}
	
}

