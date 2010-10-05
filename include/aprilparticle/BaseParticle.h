#ifndef BASE_PARTICLE_H
#define BASE_PARTICLE_H

#include "aprilparticleExport.h"
#include "Vector3.h"
#include "vector"

namespace April
{

	class aprilparticleExport BaseParticle {
		
		gvec3 position;
		gvec3 direction;
		
		float speed;
		float life;
		float size;

		public:
			BaseParticle();
			~BaseParticle();

	};

}

#endif // BASE_PARTICLE_H
