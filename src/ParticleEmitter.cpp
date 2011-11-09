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
#include "ParticleEmitter.h"
#include "Particle.h"

#define RANDOMIZE(name) (this->min ## name < this->max ## name ? hrandf(this->min ## name, this->max ## name) : this->min ## name)

namespace aprilparticle
{
	gvec3 v[4]; // optimization

	ParticleEmitter::ParticleEmitter(gvec3 position, gvec3 direction, float particleLife, float emissionRate, unsigned int max)
	{
		this->timer = 0.0f;
		this->position = position;
		this->direction = direction;
		this->dimensions.set(1.0f, 1.0f, 1.0f);
		this->minLife = particleLife;
		this->maxLife = particleLife;
		this->minSize.set(1.0f, 1.0f);
		this->maxSize.set(1.0f, 1.0f);
		this->minScale = 1.0f;
		this->maxScale = 1.0f;
		this->minSpeed = 0.0f;
		this->maxSpeed = 0.0f;
		this->minAngle = 0.0f;
		this->maxAngle = 0.0f;
		this->emissionRate = emissionRate;
		this->particleLimit = max;
		this->type = Point;
		this->blendMode = april::ADD;
		this->texture = NULL;
		this->_triangleBatch = NULL;
		this->_setupTriangleBatch();
	}

	ParticleEmitter::~ParticleEmitter()
	{
		if (this->_triangleBatch != NULL)
		{
			delete [] this->_triangleBatch;
		}
		foreach_q (Particle*, it, this->particles)
		{
			delete (*it);
		}
		foreach (Affector*, it, this->registeredAffectors)
		{
			delete (*it);
		}
	}

	void ParticleEmitter::setLife(float value)
	{
		this->minLife = value;
		this->maxLife = value;
	}

	void ParticleEmitter::setSize(gvec2 value)
	{
		this->minSize = value;
		this->maxSize = value;
	}

	void ParticleEmitter::setScale(float value)
	{
		this->minScale = value;
		this->maxScale = value;
	}

	void ParticleEmitter::setSpeed(float value)
	{
		this->minSpeed = value;
		this->maxSpeed = value;
	}

	void ParticleEmitter::setAngle(float value)
	{
		this->minAngle = value;
		this->maxAngle = value;
	}

	void ParticleEmitter::setParticleLimit(int value)
	{
		if (this->particleLimit != value)
		{
			this->particleLimit = value;
			this->_setupTriangleBatch();
		}
	}

	void ParticleEmitter::_setupTriangleBatch()
	{
		if (this->_triangleBatch != NULL)
		{
			delete [] this->_triangleBatch;
		}
		this->_triangleBatch = new april::ColoredTexturedVertex[this->particleLimit * 6];
		for (this->_i = 0; this->_i < this->particleLimit; this->_i++)
		{
			this->_triangleBatch[this->_i * 6 + 0].u = 1.0f;		this->_triangleBatch[this->_i * 6 + 0].v = 1.0f;
			this->_triangleBatch[this->_i * 6 + 1].u = 0.0f;		this->_triangleBatch[this->_i * 6 + 1].v = 1.0f;
			this->_triangleBatch[this->_i * 6 + 2].u = 1.0f;		this->_triangleBatch[this->_i * 6 + 2].v = 0.0f;
			this->_triangleBatch[this->_i * 6 + 3].u = 0.0f;		this->_triangleBatch[this->_i * 6 + 3].v = 1.0f;
			this->_triangleBatch[this->_i * 6 + 4].u = 1.0f;		this->_triangleBatch[this->_i * 6 + 4].v = 0.0f;
			this->_triangleBatch[this->_i * 6 + 5].u = 0.0f;		this->_triangleBatch[this->_i * 6 + 5].v = 0.0f;
		}
	}

	void ParticleEmitter::setLifeRange(float min, float max)
	{
		this->minLife = min;
		this->maxLife = max;
	}

	void ParticleEmitter::setSizeRange(gvec2 min, gvec2 max)
	{
		this->minSize = min;
		this->maxSize = max;
	}

	void ParticleEmitter::setScaleRange(float min, float max)
	{
		this->minScale = min;
		this->maxScale = max;
	}

	void ParticleEmitter::setSpeedRange(float min, float max)
	{
		this->minSpeed = min;
		this->maxSpeed = max;
	}

	void ParticleEmitter::setAngleRange(float min, float max)
	{
		this->minAngle = min;
		this->maxAngle = max;
	}

	void ParticleEmitter::addAffector(Affector* affector)
	{
		this->affectors += affector;
	}
	
	void ParticleEmitter::removeAffector(Affector* affector)
	{
		this->affectors -= affector;
	}
	
	void ParticleEmitter::registerAffector(Affector* affector)
	{
		this->affectors += affector;
		this->registeredAffectors += affector;
	}
	
	void ParticleEmitter::unregisterAffector(Affector* affector)
	{
		this->affectors -= affector;
		this->registeredAffectors -= affector;
	}
	
	void ParticleEmitter::_createNewParticle()
	{
		switch (this->type)
		{
			case Point:
				this->_pos = this->position;
				break;
			case Sphere:
				this->_rho = hrandf(1.0f);
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				this->_S = this->_rho * sin(this->_phi);

				this->_pos.x = this->position.x + this->_S * cos(this->_theta) * this->dimensions.x * 0.5f;
				this->_pos.y = this->position.y + this->_S * sin(this->_theta) * this->dimensions.y * 0.5f;
				this->_pos.z = this->position.z + this->_rho * cos(this->_phi) * this->dimensions.z * 0.5f;
				break;
			case HollowSphere:
				this->_rho = 1.0f;
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				this->_S = this->_rho * sin(this->_phi);

				this->_pos.x = this->position.x + this->_S * cos(this->_theta) * this->dimensions.x * 0.5f;
				this->_pos.y = this->position.y + this->_S * sin(this->_theta) * this->dimensions.y * 0.5f;
				this->_pos.z = this->position.z + this->_rho * cos(this->_phi) * this->dimensions.z * 0.5f;
				break;
			case Box:
				this->_pos.x = this->position.x + hrandf(1.0f) * this->dimensions.x - this->dimensions.x * 0.5f;
				this->_pos.y = this->position.y + hrandf(1.0f) * this->dimensions.y - this->dimensions.y * 0.5f;
				this->_pos.z = this->position.z + hrandf(1.0f) * this->dimensions.z - this->dimensions.z * 0.5f;
				break;
			case Cylinder:
				this->_rho = hrandf(1.0f);
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				this->_S = this->_rho * sin(this->_phi);
					
				this->_pos.x = this->position.x + this->_S * cos(this->_theta) * this->dimensions.x * 0.5f;
				this->_pos.y = this->position.y + hrandf(1.0f) * this->dimensions.y - this->dimensions.y * 0.5f;
				this->_pos.z = this->position.z + this->_rho * cos(this->_phi) * this->dimensions.z * 0.5f;
				break;
			case HollowCylinder:
				this->_rho = 1.0f;
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				this->_S = this->_rho * sin(this->_phi);
					
				this->_pos.x = this->position.x + this->_S * cos(this->_theta) * this->dimensions.x * 0.5f;
				this->_pos.y = this->position.y + hrandf(1.0f) * this->dimensions.y - this->dimensions.y * 0.5f;
				this->_pos.z = this->position.z + this->_rho * cos(this->_phi) * this->dimensions.z * 0.5f;
				break;
			case Ring:
				// TODO
				this->_pos = this->position;
				break;
		}
		this->_particle = new Particle(this->_pos, this->direction, RANDOMIZE(Life), gvec2(RANDOMIZE(Size.x), RANDOMIZE(Size.y)),
			RANDOMIZE(Scale), RANDOMIZE(Speed), RANDOMIZE(Angle));
		this->particles += this->_particle;
		foreach (Affector*, it, this->affectors)
		{
			(*it)->update(this->_particle, 0.0f);
		}
	}
	
	void ParticleEmitter::update(float k)
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
		if (this->emissionRate > 0.0f)
		{
			this->_cs = 1.0f / this->emissionRate;
			this->_quota = (int)(this->timer * this->emissionRate);
			if (this->_alive >= this->particleLimit)
			{
				this->timer = 0.0f;
			}
			else if (this->timer > this->_cs && this->_alive < this->particleLimit)
			{
				this->_quota = hmin(this->_quota, (int)(this->particleLimit - this->_alive));
				for (int i = 0; i < this->_quota; i++)
				{
					this->_createNewParticle();
				}
				this->timer = 0.0f;
			}
		}
	}
	
	void ParticleEmitter::draw(gvec3 point, gvec3 up)
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
	
	void ParticleEmitter::drawAffectors()
	{
		foreach (Affector*, it, this->affectors)
		{
			(*it)->draw();
		}
	}
	
}

