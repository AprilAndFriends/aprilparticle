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
			Attractor(gvec3 origin, float radius);
			~Attractor();

			gvec3 getOrigin() { return this->origin; }
			void setOrigin(gvec3 value) { this->origin = value; }
			float getRadius() { return this->radius; }
			void setRadius(float value) { this->radius = value; }
				
			void draw();
			void update(Particle* particle, double t);

		protected:
			gvec3 origin;
			float radius;

		};

	}

}

#endif
