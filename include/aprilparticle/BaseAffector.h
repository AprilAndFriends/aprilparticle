#ifndef BASE_AFFECTOR_H
#define BASE_AFFECTOR_H

#include "aprilparticleExport.h"
#include "BaseParticle.h"
#include "gtypes/Vector3.h"
#include <vector>
#include <iostream>

namespace April
{
	class ParticleEmitter;

	class aprilparticleExport BaseAffector {

		public:
			BaseAffector();
			~BaseAffector();
			
			virtual void update(ParticleEmitter *emitter, double t);

	};

}

#endif // BASE_AFFECTOR_H
