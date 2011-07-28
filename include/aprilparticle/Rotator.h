#ifndef ROTATOR_H
#define ROTATOR_H

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	class aprilparticleExport Rotator : public Affector
	{
	public:
		Rotator(float rotationSpeed = 1.0f);
		~Rotator();

		float getRotationSpeed() { return this->rotationSpeed; }
		void setRotationSpeed(float value) { this->rotationSpeed = value; }
				
		void update(Particle* particle, float k);

	protected:
		float rotationSpeed;

	};

}

#endif
