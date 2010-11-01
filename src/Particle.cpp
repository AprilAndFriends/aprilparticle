#include "Particle.h"

namespace April
{

	Particle::Particle()
	{
		mLife 					= 1.0;
		mTotalLife 				= 1.0;
		mSize 					= 1.0;
		mSpeed 					= 0.0;
		mAngle 					= 0.0;
		
		mDirection 				= gvec3(0.0,1.0,0.0);
		mPosition 				= gvec3(0.0,0.0,0.0);
		
		mColor = 0xFFFFFFFF;
	}
	
	Particle::Particle(float life, gvec3 position, gvec3 direction, float size)
	{
		mLife 					= life;
		mTotalLife 				= life;
		mSize 					= size;
		mSpeed 					= 0.0;
		mAngle 					= 0.0;
		
		mDirection 				= direction;
		mPosition 				= position;
		
		mColor = 0xFFFFFFFF;
	}
	
}


