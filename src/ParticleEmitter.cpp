#include "ParticleEmitter.h"

namespace April
{

	ParticleEmitter::ParticleEmitter()
	{
		this->life = 1.0;
		this->size = 0.5;
		
		this->randomSize = false;
		this->sizemin = 0;
		this->sizemax = 1;
		this->maxParticles = 64;
		
		this->particleDecaySpeed = 1.0;
		this->creationSpeed = 0.1;
		this->speed = 0.1;
		this->counter = 0;
		
		this->direction = gvec3(0,0,1);
	}
	
	ParticleEmitter::ParticleEmitter(float life, float size, float creationSpeed, float speed, bool rand, float smin, float smax, unsigned int max, float decay)
	{
		this->life = life;
		this->size = size;
		
		this->randomSize = rand;
		this->sizemin = smin;
		this->sizemax = smax;
		this->maxParticles = max;
		
		this->particleDecaySpeed = decay;
		this->creationSpeed = creationSpeed;
		this->speed = speed;
		this->counter = 0;
		
		this->direction = gvec3(0,0,1);
	}

	ParticleEmitter::~ParticleEmitter()
	{
	}
	
	void ParticleEmitter::setTexture(std::string texture)
	{
		//tex = April::rendersys->loadTexture(texture);
	}
	
	bool isDead(const April::ParticleEmitter& particle)
	{
		return (particle.life < 0.0);
	}
	
	void ParticleEmitter::update(float t)
	{
		counter += t;
		if((counter > creationSpeed) && (particles.size() <= maxParticles))
		{
			particles.push_back(ParticleEmitter(life,size,creationSpeed,sizemin,sizemax,maxParticles,particleDecaySpeed));
			counter = 0.0;
		}
		
		for(std::list<April::ParticleEmitter>::iterator it = particles.begin(); it != particles.end(); it++)
		{
			it->life -= particleDecaySpeed*t;
			it->position += gvec3(0,0,0.01);
		}
		
		particles.remove_if(isDead);
		
	}
	
	void ParticleEmitter::draw()
	{
		April::TexturedVertex v[4];
		//April::rendersys->setTexture(tex);
		for(std::list<April::ParticleEmitter>::iterator it = particles.begin(); it != particles.end(); it++)
		{
			v[0].x=it->position.x-size/2;	v[0].y=it->position.y-size/2;	v[0].z=it->position.z;	v[0].u=0;	v[0].v=0;
			v[1].x=it->position.x+size/2;	v[1].y=it->position.y-size/2;	v[1].z=it->position.z;	v[1].u=1;	v[1].v=0;
			v[2].x=it->position.x-size/2;	v[2].y=it->position.y+size/2;	v[2].z=it->position.z;	v[2].u=0;	v[2].v=1;
			v[3].x=it->position.x+size/2;	v[3].y=it->position.y+size/2;	v[3].z=it->position.z;	v[3].u=1;	v[3].v=1;
			
			//April::rendersys->render(April::TriangleStrip,v,4);
		}
	}

}

