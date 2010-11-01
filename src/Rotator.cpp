#include "Rotator.h"

namespace April
{

	namespace Affectors
	{

		Rotator::Rotator()
		{
			mRotationSpeed = 1.0;
		}
		
		Rotator::Rotator(float rotationSpeed)
		{
			mRotationSpeed = rotationSpeed;
		}

		Rotator::~Rotator()
		{
		}
		
		void Rotator::draw()
		{
		}
		
		void Rotator::update(Particle *particle, double t)
		{
			particle->mAngle += mRotationSpeed * t;
		}

	}

}

