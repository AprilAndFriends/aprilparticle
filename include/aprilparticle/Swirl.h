#ifndef SWIRL_H
#define SWIRL_H

#include <gtypes/Matrix3.h>
#include <gtypes/Vector3.h>

#include "Affector.h"
#include "aprilparticleExport.h"

namespace april
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport Swirl : public Affector
		{
		public:
			Swirl();
			Swirl(gvec3 position, gvec3 up, float range, float force);
			~Swirl();

			float getRange() { return this->range; }
			void setRange(float value) { this->range = value; }
			float getForce() { return this->force; }
			void setForce(float value) { this->force = value; }
			gvec3 getPosition() { return this->position; }
			void setPosition(gvec3 value) { this->position = value; }
			gvec3 getUp() { return this->up; }
			void setUp(gvec3 value) { this->up = value; }
			bool getClockwise() { return this->clockwise; }
			void setClockwise(bool value) { this->clockwise = value; }
			
			void update(april::Particle* particle, float k);

		protected:
			gmat3 _rotation;
			float range;
			float force;
			gvec3 position;
			gvec3 up;
			bool clockwise;
			
		};

	}

}

#endif
