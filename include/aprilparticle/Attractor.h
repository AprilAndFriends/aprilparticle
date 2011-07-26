#ifndef ATTRACTOR_H
#define ATTRACTOR_H

#include <gtypes/Vector3.h>

#include "Affector.h"

namespace april
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport Attractor : public Affector
		{
		public:
			Attractor();
			Attractor(gvec3 position, float radius);
			~Attractor();

			gvec3 getPosition() { return this->position; }
			void setPosition(gvec3 value) { this->position = value; }
			float getRadius() { return this->radius; }
			void setRadius(float value) { this->radius = value; }
				
			void update(Particle* particle, float k);

		protected:
			gvec3 position;
			float radius;

		};

	}

}

#endif
