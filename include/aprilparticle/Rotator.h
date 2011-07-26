#ifndef ROTATOR_H
#define ROTATOR_H

#include "Affector.h"

namespace april
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport Rotator : public Affector
		{
		public:
			Rotator(float rotationSpeed = 1.0f);
			~Rotator();

			float getRotationSpeed() { return this->rotationSpeed; }
			void setRotationSpeed(float value) { this->rotationSpeed = value; }
				
			void update(Particle* particle, double t);
			void draw();

		protected:
			float rotationSpeed;

		};

	}

}

#endif
