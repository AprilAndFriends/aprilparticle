#include "Particle.h"

namespace april
{
	Particle::Particle()
	{
		this->position = gvec3(0.0f, 0.0f, 0.0f);
		this->direction = gvec3(0.0f, 1.0f, 0.0f);
		this->life = 1.0f;
		this->totalLife = 1.0f;
		this->size = 1.0f;
		this->speed = 0.0f;
		this->angle = 0.0f;
		this->color = 0xFFFFFFFF;
	}
	
	Particle::Particle(float life, gvec3 position, gvec3 direction, float size)
	{
		this->position = position;
		this->direction = direction;
		this->life = life;
		this->totalLife = life;
		this->size = size;
		this->speed = 0.0f;
		this->angle = 0.0f;
		this->color = 0xFFFFFFFF;
	}
	
	Particle::~Particle()
	{
	}
	
}


