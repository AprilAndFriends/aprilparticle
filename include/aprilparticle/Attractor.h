#ifndef _ATTRACTOR_H_
#define _ATTRACTOR_H_

#include "Affector.h"
#include "Particle.h"

namespace April
{

	namespace Affectors
	{

		class aprilparticleExport Attractor : public Affector
		{

			public:
			
			gvec3 mOrigin;
			float mRadius;
			
				Attractor();
				Attractor(gvec3 origin, float radius);
				~Attractor();
				
				void draw();
				void update(Particle *particle, double t);

		};

	}

}

#endif // _ATTRACTOR_H_
