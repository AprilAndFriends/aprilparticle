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
		this->size.set(1.0f, 1.0f, 1.0f);
		this->minParticleLife = particleLife;
		this->maxParticleLife = particleLife;
		this->minParticleSize.set(1.0f, 1.0f);
		this->maxParticleSize.set(1.0f, 1.0f);
		this->minParticleScale = 1.0f;
		this->maxParticleScale = 1.0f;
		this->minParticleSpeed = 0.0f;
		this->maxParticleSpeed = 0.0f;
		this->minParticleAngle = 0.0f;
		this->maxParticleAngle = 0.0f;
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
		foreach (Affector*, it, this->registeredAffectors)
		{
			delete (*it);
		}
	}

	void ParticleEmitter::setParticleLife(float value)
	{
		this->minParticleLife = value;
		this->maxParticleLife = value;
	}

	void ParticleEmitter::setParticleSize(gvec2 value)
	{
		this->minParticleSize = value;
		this->maxParticleSize = value;
	}

	void ParticleEmitter::setParticleScale(float value)
	{
		this->minParticleScale = value;
		this->maxParticleScale = value;
	}

	void ParticleEmitter::setParticleSpeed(float value)
	{
		this->minParticleSpeed = value;
		this->maxParticleSpeed = value;
	}

	void ParticleEmitter::setParticleAngle(float value)
	{
		this->minParticleAngle = value;
		this->maxParticleAngle = value;
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

	void ParticleEmitter::setParticleLifeRange(float min, float max)
	{
		this->minParticleLife = min;
		this->maxParticleLife = max;
	}

	void ParticleEmitter::setParticleSizeRange(gvec2 min, gvec2 max)
	{
		this->minParticleSize = min;
		this->maxParticleSize = max;
	}

	void ParticleEmitter::setParticleScaleRange(float min, float max)
	{
		this->minParticleScale = min;
		this->maxParticleScale = max;
	}

	void ParticleEmitter::setParticleSpeedRange(float min, float max)
	{
		this->minParticleSpeed = min;
		this->maxParticleSpeed = max;
	}

	void ParticleEmitter::setParticleAngleRange(float min, float max)
	{
		this->minParticleAngle = min;
		this->maxParticleAngle = max;
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
	
	void ParticleEmitter::createParticle()
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

				this->_pos.x = this->position.x + this->_S * cos(this->_theta) * this->size.x * 0.5f;
				this->_pos.y = this->position.y + this->_S * sin(this->_theta) * this->size.y * 0.5f;
				this->_pos.z = this->position.z + this->_rho * cos(this->_phi) * this->size.z * 0.5f;
				break;
			case HollowSphere:
				this->_rho = 1.0f;
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				this->_S = this->_rho * sin(this->_phi);

				this->_pos.x = this->position.x + this->_S * cos(this->_theta) * this->size.x * 0.5f;
				this->_pos.y = this->position.y + this->_S * sin(this->_theta) * this->size.y * 0.5f;
				this->_pos.z = this->position.z + this->_rho * cos(this->_phi) * this->size.z * 0.5f;
				break;
			case Box:
				this->_pos.x = this->position.x + hrandf(1.0f) * this->size.x - this->size.x * 0.5f;
				this->_pos.y = this->position.y + hrandf(1.0f) * this->size.y - this->size.y * 0.5f;
				this->_pos.z = this->position.z + hrandf(1.0f) * this->size.z - this->size.z * 0.5f;
				break;
			case Cylinder:
				this->_rho = hrandf(1.0f);
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				this->_S = this->_rho * sin(this->_phi);
					
				this->_pos.x = this->position.x + this->_S * cos(this->_theta) * this->size.x * 0.5f;
				this->_pos.y = this->position.y + hrandf(1.0f) * this->size.y - this->size.y * 0.5f;
				this->_pos.z = this->position.z + this->_rho * cos(this->_phi) * this->size.z * 0.5f;
				break;
			case HollowCylinder:
				this->_rho = 1.0f;
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				this->_S = this->_rho * sin(this->_phi);
					
				this->_pos.x = this->position.x + this->_S * cos(this->_theta) * this->size.x * 0.5f;
				this->_pos.y = this->position.y + hrandf(1.0f) * this->size.y - this->size.y * 0.5f;
				this->_pos.z = this->position.z + this->_rho * cos(this->_phi) * this->size.z * 0.5f;
				break;
			case Ring:
				// TODO
				this->_pos = this->position;
				break;
		}
		this->particles += Particle(this->_pos, this->direction, RANDOMIZE(ParticleLife), gvec2(RANDOMIZE(ParticleSize.x), RANDOMIZE(ParticleSize.y)),
			RANDOMIZE(ParticleScale), RANDOMIZE(ParticleSpeed), RANDOMIZE(ParticleAngle));
	}
	
	void ParticleEmitter::update(float k)
	{
		// first remove all expired particles
		while (this->particles.size() > 0)
		{
			this->particles.front().timer += k;
			if (this->particles.front().isDead())
			{
				this->particles.pop_front();
			}
			else
			{
				this->particles.front().timer -= k;
				break;
			}
		}

		this->timer += k;
		if (this->emissionRate > 0.0f)
		{
			this->_cs = 1.0f / this->emissionRate;
			this->_quota = (int)(this->timer * this->emissionRate);
			if (this->particles.size() >= this->particleLimit)
			{
				this->timer = 0.0f;
			}
			else if (this->timer > this->_cs && this->particles.size() < this->particleLimit)
			{
				this->_quota = hmin(this->_quota, (int)(this->particleLimit - this->particles.size()));
				for (int i = 0; i < this->_quota; i++)
				{
					this->createParticle();
				}
				this->timer = 0.0f;
			}
		}
		
		foreach_q (Particle, it, this->particles)
		{
			(*it).timer += k;
			if (!(*it).isDead())
			{
				foreach (Affector*, it2, this->affectors)
				{
					(*it2)->update(&(*it), k);
				}
			}
		}
	}
	
	void ParticleEmitter::draw(gvec3 point, gvec3 up)
	{
		this->_i = 0;
		foreach_q (Particle, it, this->particles)
		{
			if (!(*it).isDead())
			{
				this->_billboard.lookAt((*it).position, point - (*it).position, up);
				v[0].set(-(*it).size.x * (*it).scale * 0.5f, -(*it).size.y * (*it).scale * 0.5f, 0.0f);
				v[1].set((*it).size.x * (*it).scale * 0.5f, -(*it).size.y * (*it).scale * 0.5f, 0.0f);
				v[2].set(-(*it).size.x * (*it).scale * 0.5f, (*it).size.y * (*it).scale * 0.5f, 0.0f);
				v[3].set((*it).size.x * (*it).scale * 0.5f, (*it).size.y * (*it).scale * 0.5f, 0.0f);
			
				this->_rot.setRotation3D(0.0f, 0.0f, 1.0f, (*it).angle);
				v[0] = this->_rot * v[0];
				v[1] = this->_rot * v[1];
				v[2] = this->_rot * v[2];
				v[3] = this->_rot * v[3];
			
				this->_billboard.inverse();
				v[0] = this->_billboard * v[0];
				v[1] = this->_billboard * v[1];
				v[2] = this->_billboard * v[2];
				v[3] = this->_billboard * v[3];
			
				this->_color = (unsigned int)(*it).color;
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

