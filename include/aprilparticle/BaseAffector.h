#ifndef BASE_AFFECTOR_H
#define BASE_AFFECTOR_H

#include "aprilparticleExport.h"
#include "BaseParticle.h"
#include "ParticleEmitter.h"
#include "gtypes/Vector3.h"
#include <vector>
#include <iostream>

namespace April
{

	class aprilparticleExport BaseAffector {
		
			float affectingRadius;
			std::vector<April::ParticleEmitter*> asignedEmitters;

		public:
			BaseAffector();
			~BaseAffector();
			
			virtual void update(float k);
			void assignEmitter(April::ParticleEmitter *emitter);

	};

}

#endif // BASE_AFFECTOR_H
