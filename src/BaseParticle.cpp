#include "BaseParticle.h"

namespace April
{

	BaseParticle::BaseParticle()
	{
		this->life = 0;
		this->size = 0;
	}
	
	BaseParticle::BaseParticle(float life, float size)
	{
		this->life = life;
		this->size = size;
	}
	
}


