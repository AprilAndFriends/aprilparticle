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
			
			void draw();
			void update(april::Particle* particle, double t);

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
