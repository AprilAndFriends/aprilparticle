#ifndef BASE_PARTICLE_H
#define BASE_PARTICLE_H

#include "aprilparticleExport.h"
#include "gtypes/Vector3.h"
#include "vector"

namespace April
{
	class aprilparticleExport BaseParticle {

		public:
		
			gvec3 position;
			gvec3 direction;
			
			float life;
			float size;
		
			BaseParticle();
			BaseParticle(float life, float size);
	};

}

#endif // BASE_PARTICLE_H
