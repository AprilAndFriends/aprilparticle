#include <math.h>

#include <gtypes/Constants.h>
#include <gtypes/Matrix3.h>
#include <gtypes/Matrix4.h>
#include <gtypes/Vector2.h>
#include <hltypes/util.h>

#include "ParticleEmitter.h"

namespace april
{
	april::ColoredTexturedVertex v[6]; // optimization

	ParticleEmitter::ParticleEmitter()
	{
		this->blendMode = april::ADD;
		this->emitterType = april::ET_Point;
		
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
		this->_triangleBatch = new april::ColoredTexturedVertex[this->maxParticles * 6];
		this->life = 1.0f;
		this->lifeMax = 1.0f;
		this->lifeMin = 1.0f;
		this->texture = NULL;
	}
	
	ParticleEmitter::ParticleEmitter(float life, float particlesPerSecond, gvec3 position, gvec3 direction, unsigned int max)
	{
		this->blendMode = april::ADD;
		this->emitterType = april::ET_Point;
		
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
		this->_triangleBatch = new april::ColoredTexturedVertex[this->maxParticles * 6];
		this->life = life;
		this->lifeMax = life;
		this->lifeMin = life;
		this->texture = NULL;
	}

	ParticleEmitter::~ParticleEmitter()
	{
		delete [] _triangleBatch;
	}
	
	void ParticleEmitter::setLifeRange(float lifeMin, float lifeMax)
	{
		this->lifeMin = lifeMin;
		this->lifeMax = lifeMax;
	}

	void ParticleEmitter::setSizeRange(float sizeMin, float sizeMax)
	{
		this->minSize = sizeMin;
		this->maxSize = sizeMax;
	}
	
	/*bool isDead(const april::Particle& particle)
	{
		if(particle.mLife < 0.0)
		{
			return true;
		}
		return false;
	}*/
	
	void ParticleEmitter::createParticle()
	{
		gvec3 npos;
		switch (this->emitterType)
		{
			case april::ET_Point:
			{
				npos = this->position;
				break;
			}
			case april::ET_Sphere:
			{
				float rho, phi, theta, S;
					
				rho = hrandf(1.0f);
				phi = hrandf((float)(2 * G_PI));
				theta = hrandf((float)G_PI);
					
				S = rho * sin(phi);
				npos.x = this->position.x + S * cos(theta) * this->length * 0.5f;
				npos.y = this->position.y + S * sin(theta) * this->height * 0.5f;
				npos.z = this->position.z + rho * cos(phi) * this->width * 0.5f;
				break;
			}
			case april::ET_HollowSphere:
			{
				float rho, phi, theta, S;
					
				rho = 1.0f;
				phi = hrandf((float)(2 * G_PI));
				theta = hrandf((float)G_PI);
					
				S = rho * sin(phi);
				npos.x = this->position.x + S * cos(theta) * this->length * 0.5f;
				npos.y = this->position.y + S * sin(theta) * this->height * 0.5f;
				npos.z = this->position.z + rho * cos(phi) * this->width * 0.5f;
				break;
			}
			case april::ET_Box:
			{
				npos.x = this->position.x + ((float)(rand()) / RAND_MAX) * this->length - this->length * 0.5f;
				npos.y = this->position.y + ((float)(rand()) / RAND_MAX) * this->height - this->height * 0.5f;
				npos.z = this->position.z + ((float)(rand()) / RAND_MAX) * this->width - this->width * 0.5f;
				break;
			}
			case april::ET_Cylinder:
			{
				float rho, phi, theta, S;
					
				rho = hrandf(1.0f);
				phi = hrandf((float)(2 * G_PI));
				theta = hrandf((float)G_PI);
					
				S = rho * sin(phi);
					
				npos.x = this->position.x + S * cos(theta) * this->length * 0.5f;
				npos.y = this->position.y + ((float)(rand()) / RAND_MAX) * this->height - this->height * 0.5f;
				npos.z = this->position.z + rho * cos(phi) * this->width * 0.5f;
				break;
			}
			case april::ET_HollowCylinder:
			{
				float rho, phi, theta, S;
					
				rho = 1.0f;
				phi = hrandf((float)(2 * G_PI));
				theta = hrandf((float)G_PI);
					
				S = rho * sin(phi);
					
				npos.x = this->position.x + S * cos(theta) * this->length * 0.5f;
				npos.y = this->position.y + ((float)(rand()) / RAND_MAX) * this->height - this->height * 0.5f;
				npos.z = this->position.z + rho * cos(phi) * this->width * 0.5f;
				break;
			}
			case april::ET_Ring:
			{
				// TODO
				npos = this->position;
				break;
			}
		}

		Particle particle(npos, this->direction, this->life, 0.0f);
		particle.setSize(this->randomStartSize ? hrandf(this->minSize, this->maxSize) : this->size);
		particle.setAngle(hrandf((float)(2 * G_PI)));
		this->particles.push_back(particle);
	}
	
	void ParticleEmitter::update(float k)
	{
		this->counter += k;
		if (this->particlesPerSecond > 0)
		{
			float cs = 1.0f / this->particlesPerSecond;
			int quota = (int)(this->counter / cs);
			if (this->particles.size() >= this->maxParticles)
			{
				this->counter = 0.0f;
			}
			if (this->counter > cs && this->particles.size() < this->maxParticles)
			{
				for (int i = 0; i < quota; i++)
				{
					createParticle();
				}
				this->counter = 0.0f;
			}
		}
		
		// TODO - change to a foreach_q iterator
		for (std::deque<april::Particle>::iterator it = this->particles.begin(); it != this->particles.end(); it++)
		{
			(*it).setLife((*it).getLife() - k);
			for (hlist<Affectors::Affector*>::iterator jt = this->affectors.begin(); jt != this->affectors.end(); jt++)
			{
				(*jt)->update(&(*it), k);
			}
		}
		while (this->particles.size() > 0 && this->particles.front().getLife() < 0.0f)
		{
			this->particles.pop_front();
		}
	}
	
	void ParticleEmitter::addAffector(Affectors::Affector* affector)
	{
		// TODO - change to operator+=
		this->affectors.push_back(affector);
	}
	
	void ParticleEmitter::draw(gvec3 point, gvec3 up)
	{
		gmat4 billboard;
		gvec3 v0;
		gvec3 v1;
		gvec3 v2;
		gvec3 v3;
		gmat3 rot;
		float s;
		
		v[0].u = 1.0f;	v[0].v = 1.0f;
		v[1].u = 0.0f;	v[1].v = 1.0f;
		v[2].u = 1.0f;	v[2].v = 0.0f;
		v[3].u = 0.0f;	v[3].v = 1.0f;
		v[4].u = 1.0f;	v[4].v = 0.0f;
		v[5].u = 0.0f;	v[5].v = 0.0f;
		
		int i = 0;
		unsigned int color;
		for (std::deque<april::Particle>::iterator it = this->particles.begin(); it != this->particles.end(); i++, it++)
		{
			billboard.lookAt((*it).getPosition(), point - (*it).getPosition(), up);
			s = (*it).getSize();
			v0 = gvec3(-s / 2, -s / 2, 0.0f);
			v1 = gvec3(s / 2, -s / 2, 0.0f);
			v2 = gvec3(-s / 2, s / 2, 0.0f);
			v3 = gvec3(s / 2, s / 2, 0.0f);
			
			rot.setRotation3D(0.0f, 0.0f, 1.0f, (*it).getAngle());
			v0 = rot * v0;
			v1 = rot * v1;
			v2 = rot * v2;
			v3 = rot * v3;
			
			billboard.inverse();
			v0 = billboard * v0;
			v1 = billboard * v1;
			v2 = billboard * v2;
			v3 = billboard * v3;
			
			color = (unsigned int)(*it).getColor();
			v[0] = v0;	v[0].color = color;
			v[1] = v1;	v[1].color = color;
			v[2] = v2;	v[2].color = color;
			
			v[3] = v1;	v[3].color = color;
			v[4] = v2;	v[4].color = color;
			v[5] = v3;	v[5].color = color;
			
			this->_triangleBatch[i * 6 + 0] = v[0];
			this->_triangleBatch[i * 6 + 1] = v[1];
			this->_triangleBatch[i * 6 + 2] = v[2];
			this->_triangleBatch[i * 6 + 3] = v[3];
			this->_triangleBatch[i * 6 + 4] = v[4];
			this->_triangleBatch[i * 6 + 5] = v[5];
			
		}
		if (this->texture != NULL)
		{
			april::rendersys->setTexture(this->texture);
		}
		april::rendersys->setBlendMode(this->blendMode);
		april::rendersys->render(april::TriangleList, this->_triangleBatch, this->particles.size() * 6);
	}
	
	void ParticleEmitter::drawAffectors()
	{
		foreach_l (Affectors::Affector*, it, this->affectors)
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
			delete [] this->_triangleBatch;
			this->_triangleBatch = new april::ColoredTexturedVertex[this->maxParticles * 6];
		}
	}

}

