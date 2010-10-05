#ifndef BASE_AFFECTOR_H
#define BASE_AFFECTOR_H

#include "aprilparticleExport.h"
#include "BaseParticle.h"
#include "Vector3.h"

namespace April
{

	class aprilparticleExport BaseAffector : public BaseParticle {
		
			float affectingRadius;
			

		public:
			BaseAffector();
			~BaseAffector();

	};

}

#endif // BASE_AFFECTOR_H
