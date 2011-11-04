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

#include "ParticleEmitter.h"

namespace aprilparticle
{
	gvec3 v[4]; // optimization

	ParticleEmitter::ParticleEmitter() : Particle()
	{
		this->blendMode = april::ADD;
		this->emitterType = Point;
		
		this->width = 1.0f;
		this->height = 1.0f;
		this->length = 1.0f;
		
		this->scale = 1.0f;
		this->minParticleScale = 0.8f;
		this->maxParticleScale = 1.2f;
			
		this->emissionRate = 60.0f;
		this->timer = 0.0f;
		this->maxParticles = 10;
		this->_triangleBatch = NULL;
		this->_setupTriangleBatch();
		this->life = 1.0f;
		this->minLife = 1.0f;
		this->maxLife = 1.0f;
		this->texture = NULL;
	}
	
	ParticleEmitter::ParticleEmitter(float life, float emissionRate, gvec3 position, gvec3 direction, unsigned int max)
	{
		this->blendMode = april::ADD;
		this->emitterType = Point;
		
		this->position = position;
		this->direction = direction;
		
		this->width = 1.0f;
		this->height = 1.0f;
		this->length = 1.0f;
		
		this->scale = 1.0f;
		this->minParticleScale = 0.8f;
		this->maxParticleScale = 1.2f;
			
		this->emissionRate = emissionRate;
		this->timer = 0.0f;
		this->maxParticles = max;
		this->_triangleBatch = NULL;
		this->_setupTriangleBatch();
		this->life = life;
		this->minLife = life;
		this->maxLife = life;
		this->texture = NULL;
	}

	ParticleEmitter::~ParticleEmitter()
	{
		if (this->_triangleBatch != NULL)
		{
			delete [] this->_triangleBatch;
		}
	}

	void ParticleEmitter::setParticleScale(float value)
	{
		this->minParticleScale = value;
		this->maxParticleScale = value;
	}

	void ParticleEmitter::setParticleScaleRange(float min, float max)
	{
		this->minParticleScale = min;
		this->maxParticleScale = max;
	}

	void ParticleEmitter::setParticleAngleRange(float min, float max)
	{
		// TODO
		//this->minParticleAngle = min;
		//this->maxParticleAngle = max;
	}


	
	void ParticleEmitter::setLifeRange(float minLife, float maxLife)
	{
		this->minLife = minLife;
		this->maxLife = maxLife;
	}

	void ParticleEmitter::setSizeRange(float minParticleScale, float maxParticleScale)
	{
		this->minParticleScale = minParticleScale;
		this->maxParticleScale = maxParticleScale;
	}
	
	void ParticleEmitter::createParticle()
	{
		switch (this->emitterType)
		{
			case Point:
				this->_pos = this->position;
				break;
			case Sphere:
				this->_rho = hrandf(1.0f);
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				this->_S = this->_rho * sin(this->_phi);

				this->_pos.x = this->position.x + this->_S * cos(this->_theta) * this->length * 0.5f;
				this->_pos.y = this->position.y + this->_S * sin(this->_theta) * this->height * 0.5f;
				this->_pos.z = this->position.z + this->_rho * cos(this->_phi) * this->width * 0.5f;
				break;
			case HollowSphere:
				this->_rho = 1.0f;
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				this->_S = this->_rho * sin(this->_phi);

				this->_pos.x = this->position.x + this->_S * cos(this->_theta) * this->length * 0.5f;
				this->_pos.y = this->position.y + this->_S * sin(this->_theta) * this->height * 0.5f;
				this->_pos.z = this->position.z + this->_rho * cos(this->_phi) * this->width * 0.5f;
				break;
			case Box:
				this->_pos.x = this->position.x + hrandf(1.0f) * this->length - this->length * 0.5f;
				this->_pos.y = this->position.y + hrandf(1.0f) * this->height - this->height * 0.5f;
				this->_pos.z = this->position.z + hrandf(1.0f) * this->width - this->width * 0.5f;
				break;
			case Cylinder:
				this->_rho = hrandf(1.0f);
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				this->_S = this->_rho * sin(this->_phi);
					
				this->_pos.x = this->position.x + this->_S * cos(this->_theta) * this->length * 0.5f;
				this->_pos.y = this->position.y + hrandf(1.0f) * this->height - this->height * 0.5f;
				this->_pos.z = this->position.z + this->_rho * cos(this->_phi) * this->width * 0.5f;
				break;
			case HollowCylinder:
				this->_rho = 1.0f;
				this->_phi = hrandf((float)G_PIx2);
				this->_theta = hrandf((float)G_PI);
				this->_S = this->_rho * sin(this->_phi);
					
				this->_pos.x = this->position.x + this->_S * cos(this->_theta) * this->length * 0.5f;
				this->_pos.y = this->position.y + hrandf(1.0f) * this->height - this->height * 0.5f;
				this->_pos.z = this->position.z + this->_rho * cos(this->_phi) * this->width * 0.5f;
				break;
			case Ring:
				// TODO
				this->_pos = this->position;
				break;
		}
		this->particles += Particle(this->_pos, this->direction, this->life,
			(this->minParticleScale < this->maxParticleScale ? hrandf(this->minParticleScale, this->maxParticleScale) : this->minParticleScale), 0.0f, hrandf((float)G_PIx2));
	}
	
	void ParticleEmitter::update(float k)
	{
		// first remove all expired particles
		while (this->particles.size() > 0)
		{
			this->particles.front().life -= k;
			if (this->particles.front().isDead())
			{
				this->particles.pop_front();
			}
			else
			{
				this->particles.front().life += k;
				break;
			}
		}

		this->timer += k;
		if (this->emissionRate > 0.0f)
		{
			this->_cs = 1.0f / this->emissionRate;
			this->_quota = (int)(this->timer * this->emissionRate);
			if (this->particles.size() >= this->maxParticles)
			{
				this->timer = 0.0f;
			}
			else if (this->timer > this->_cs && this->particles.size() < this->maxParticles)
			{
				this->_quota = hmin(this->_quota, (int)(this->maxParticles - this->particles.size()));
				for (int i = 0; i < this->_quota; i++)
				{
					this->createParticle();
				}
				this->timer = 0.0f;
			}
		}
		
		foreach_q (Particle, it, this->particles)
		{
			(*it).life -= k;
			if (!(*it).isDead())
			{
				foreach_l (Affector*, it2, this->affectors)
				{
					(*it2)->update(&(*it), k);
				}
			}
		}
	}
	
	void ParticleEmitter::addAffector(Affector* affector)
	{
		this->affectors += affector;
	}
	
	void ParticleEmitter::setEmitterVolume(float width, float height, float length)
	{
		this->width = width;
		this->height = height;
		this->length = length;
	}
	
	void ParticleEmitter::setMaxParticles(int value)
	{
		if (this->maxParticles != value)
		{
			this->maxParticles = value;
			this->_setupTriangleBatch();
		}
	}

	void ParticleEmitter::_setupTriangleBatch()
	{
		if (this->_triangleBatch != NULL)
		{
			delete [] this->_triangleBatch;
		}
		this->_triangleBatch = new april::ColoredTexturedVertex[this->maxParticles * 6];
		for (this->_i = 0; this->_i < this->maxParticles; this->_i++)
		{
			this->_triangleBatch[this->_i * 6 + 0].u = 1.0f;	this->_triangleBatch[this->_i * 6 + 0].v = 1.0f;
			this->_triangleBatch[this->_i * 6 + 1].u = 0.0f;	this->_triangleBatch[this->_i * 6 + 1].v = 1.0f;
			this->_triangleBatch[this->_i * 6 + 2].u = 1.0f;	this->_triangleBatch[this->_i * 6 + 2].v = 0.0f;
			this->_triangleBatch[this->_i * 6 + 3].u = 0.0f;	this->_triangleBatch[this->_i * 6 + 3].v = 1.0f;
			this->_triangleBatch[this->_i * 6 + 4].u = 1.0f;	this->_triangleBatch[this->_i * 6 + 4].v = 0.0f;
			this->_triangleBatch[this->_i * 6 + 5].u = 0.0f;	this->_triangleBatch[this->_i * 6 + 5].v = 0.0f;
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
				v[0].set(-(*it).scale / 2, -(*it).scale / 2, 0.0f);
				v[1].set((*it).scale / 2, -(*it).scale / 2, 0.0f);
				v[2].set(-(*it).scale / 2, (*it).scale / 2, 0.0f);
				v[3].set((*it).scale / 2, (*it).scale / 2, 0.0f);
			
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
		foreach_l (Affector*, it, this->affectors)
		{
			(*it)->draw();
		}
	}
	
}

