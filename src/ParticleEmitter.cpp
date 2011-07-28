#include <math.h>

#include <gtypes/Constants.h>
#include <gtypes/Matrix3.h>
#include <gtypes/Matrix4.h>
#include <gtypes/Vector2.h>
#include <hltypes/util.h>

#include "ParticleEmitter.h"

namespace aprilparticle
{
	gvec3 v[4]; // optimization

	ParticleEmitter::ParticleEmitter()
	{
		this->blendMode = april::ADD;
		this->emitterType = ET_Point;
		
		this->position = gvec3(0.0f, 0.0f, 0.0f);
		this->direction = gvec3(0.0f, 1.0f, 0.0f);
		
		this->width = 1.0f;
		this->height = 1.0f;
		this->length = 1.0f;
		
		this->randomStartAngle = false;
		this->randomStartSize = false;
		this->randomLife = false;
		
		this->size = 1.0f;
		this->minSize = 0.8f;
		this->maxSize = 1.2f;
			
		this->particlesPerSecond	 = 60.0f;
		this->counter = 0.0f;
		this->maxParticles = 10;
		this->_triangleBatch = NULL;
		this->_setupTriangleBatch();
		this->life = 1.0f;
		this->minLife = 1.0f;
		this->maxLife = 1.0f;
		this->texture = NULL;
	}
	
	ParticleEmitter::ParticleEmitter(float life, float particlesPerSecond, gvec3 position, gvec3 direction, unsigned int max)
	{
		this->blendMode = april::ADD;
		this->emitterType = ET_Point;
		
		this->position = position;
		this->direction = direction;
		
		this->width = 1.0f;
		this->height = 1.0f;
		this->length = 1.0f;
		
		this->randomStartAngle = false;
		this->randomStartSize = false;
		this->randomLife = false;
		
		this->size = 1.0f;
		this->minSize = 0.8f;
		this->maxSize = 1.2f;
			
		this->particlesPerSecond	 = particlesPerSecond;
		this->counter = 0.0f;
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
		delete [] _triangleBatch;
	}
	
	void ParticleEmitter::setLifeRange(float minLife, float maxLife)
	{
		this->minLife = minLife;
		this->maxLife = maxLife;
	}

	void ParticleEmitter::setSizeRange(float minSize, float maxSize)
	{
		this->minSize = minSize;
		this->maxSize = maxSize;
	}
	
	void ParticleEmitter::createParticle()
	{
		gvec3 position;
		switch (this->emitterType)
		{
			case ET_Point:
			{
				position = this->position;
				break;
			}
			case ET_Sphere:
			{
				float rho, phi, theta, S;
					
				rho = hrandf(1.0f);
				phi = hrandf((float)(2 * G_PI));
				theta = hrandf((float)G_PI);
					
				S = rho * sin(phi);
				position.x = this->position.x + S * cos(theta) * this->length * 0.5f;
				position.y = this->position.y + S * sin(theta) * this->height * 0.5f;
				position.z = this->position.z + rho * cos(phi) * this->width * 0.5f;
				break;
			}
			case ET_HollowSphere:
			{
				float rho, phi, theta, S;
					
				rho = 1.0f;
				phi = hrandf((float)(2 * G_PI));
				theta = hrandf((float)G_PI);
					
				S = rho * sin(phi);
				position.x = this->position.x + S * cos(theta) * this->length * 0.5f;
				position.y = this->position.y + S * sin(theta) * this->height * 0.5f;
				position.z = this->position.z + rho * cos(phi) * this->width * 0.5f;
				break;
			}
			case ET_Box:
			{
				position.x = this->position.x + hrandf(1.0f) * this->length - this->length * 0.5f;
				position.y = this->position.y + hrandf(1.0f) * this->height - this->height * 0.5f;
				position.z = this->position.z + hrandf(1.0f) * this->width - this->width * 0.5f;
				break;
			}
			case ET_Cylinder:
			{
				float rho, phi, theta, S;
					
				rho = hrandf(1.0f);
				phi = hrandf((float)(2 * G_PI));
				theta = hrandf((float)G_PI);
					
				S = rho * sin(phi);
					
				position.x = this->position.x + S * cos(theta) * this->length * 0.5f;
				position.y = this->position.y + hrandf(1.0f) * this->height - this->height * 0.5f;
				position.z = this->position.z + rho * cos(phi) * this->width * 0.5f;
				break;
			}
			case ET_HollowCylinder:
			{
				float rho, phi, theta, S;
					
				rho = 1.0f;
				phi = hrandf((float)(2 * G_PI));
				theta = hrandf((float)G_PI);
					
				S = rho * sin(phi);
					
				position.x = this->position.x + S * cos(theta) * this->length * 0.5f;
				position.y = this->position.y + hrandf(1.0f) * this->height - this->height * 0.5f;
				position.z = this->position.z + rho * cos(phi) * this->width * 0.5f;
				break;
			}
			case ET_Ring:
			{
				// TODO
				position = this->position;
				break;
			}
		}

		Particle particle(position, this->direction, this->life, 0.0f);
		particle.size = this->randomStartSize ? hrandf(this->minSize, this->maxSize) : this->size;
		particle.angle = hrandf((float)(2 * G_PI));
		this->particles.push_back(particle);
	}
	
	void ParticleEmitter::update(float k)
	{
		this->counter += k;
		if (this->particlesPerSecond > 0.0f)
		{
			float cs = 1.0f / this->particlesPerSecond;
			int quota = (int)(this->counter / cs);
			if (this->particles.size() >= this->maxParticles)
			{
				this->counter = 0.0f;
			}
			if (this->counter > cs && this->particles.size() < this->maxParticles)
			{
				quota = hmin(quota, (int)(this->maxParticles - this->particles.size()));
				for (int i = 0; i < quota; i++)
				{
					this->createParticle();
				}
				this->counter = 0.0f;
			}
		}
		
		// first remove all expired particles
		while (this->particles.size() > 0)
		{
			this->particles[0].life -= k;
			if (this->particles[0].isDead())
			{
				this->particles.pop_front();
			}
			else
			{
				break;
			}
		}
		// TODO - change to a foreach_q iterator
		for (std::deque<Particle>::iterator it = this->particles.begin(); it != this->particles.end(); it++)
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
		// TODO - change to operator+=
		this->affectors.push_back(affector);
	}
	
	void ParticleEmitter::draw(gvec3 point, gvec3 up)
	{
		gmat4 billboard;
		gmat3 rot;
		int i = 0;
		unsigned int color;
		for (std::deque<Particle>::iterator it = this->particles.begin(); it != this->particles.end(); i++, it++)
		{
			if ((*it).isDead())
			{
				i--;
				continue;
			}
			billboard.lookAt((*it).position, point - (*it).position, up);
			v[0].set(-(*it).size / 2, -(*it).size / 2, 0.0f);
			v[1].set((*it).size / 2, -(*it).size / 2, 0.0f);
			v[2].set(-(*it).size / 2, (*it).size / 2, 0.0f);
			v[3].set((*it).size / 2, (*it).size / 2, 0.0f);
			
			rot.setRotation3D(0.0f, 0.0f, 1.0f, (*it).angle);
			v[0] = rot * v[0];
			v[1] = rot * v[1];
			v[2] = rot * v[2];
			v[3] = rot * v[3];
			
			billboard.inverse();
			v[0] = billboard * v[0];
			v[1] = billboard * v[1];
			v[2] = billboard * v[2];
			v[3] = billboard * v[3];
			
			color = (unsigned int)(*it).color;
			this->_triangleBatch[i * 6 + 0] = v[0];	this->_triangleBatch[i * 6 + 0].color = color;
			this->_triangleBatch[i * 6 + 1] = v[1];	this->_triangleBatch[i * 6 + 1].color = color;
			this->_triangleBatch[i * 6 + 2] = v[2];	this->_triangleBatch[i * 6 + 2].color = color;
			this->_triangleBatch[i * 6 + 3] = v[1];	this->_triangleBatch[i * 6 + 3].color = color;
			this->_triangleBatch[i * 6 + 4] = v[2];	this->_triangleBatch[i * 6 + 4].color = color;
			this->_triangleBatch[i * 6 + 5] = v[3];	this->_triangleBatch[i * 6 + 5].color = color;
		}
		if (this->texture != NULL)
		{
			april::rendersys->setTexture(this->texture);
		}
		april::rendersys->setBlendMode(this->blendMode);
		april::rendersys->render(april::TriangleList, this->_triangleBatch, i * 6);
	}
	
	void ParticleEmitter::drawAffectors()
	{
		foreach_l (Affector*, it, this->affectors)
		{
			(*it)->draw();
		}
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
		for (unsigned int i = 0; i < this->maxParticles; i++)
		{
			this->_triangleBatch[i * 6 + 0].u = 1.0f;	this->_triangleBatch[i * 6 + 0].v = 1.0f;
			this->_triangleBatch[i * 6 + 1].u = 0.0f;	this->_triangleBatch[i * 6 + 1].v = 1.0f;
			this->_triangleBatch[i * 6 + 2].u = 1.0f;	this->_triangleBatch[i * 6 + 2].v = 0.0f;
			this->_triangleBatch[i * 6 + 3].u = 0.0f;	this->_triangleBatch[i * 6 + 3].v = 1.0f;
			this->_triangleBatch[i * 6 + 4].u = 1.0f;	this->_triangleBatch[i * 6 + 4].v = 0.0f;
			this->_triangleBatch[i * 6 + 5].u = 0.0f;	this->_triangleBatch[i * 6 + 5].v = 0.0f;
		}
	}

}

