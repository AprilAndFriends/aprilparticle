#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "aprilparticleExport.h"
#include "BaseParticle.h"
#include "BaseAffector.h"
#include <string>


namespace April
{

	class aprilparticleExport ParticleEmitter : public BaseParticle {

			std::vector<ParticleEmitter> children;
			ParticleEmitter parent; 	// reference to the parent object
			
			bool randomSize;			// used to define wther to use random size on children particles
			float sizemin, sizemax;		// if randomSize is true use this as a size range
			
			unsigned int maxChildren;	// max number of children alive
			float childrenDecaySpeed;	// decya speed for children
			
			std::vector<BaseAffector> affectors;	// references to the affectors that affect this emitters children
		
		public:
		
			ParticleEmitter();
			~ParticleEmitter();
			
			void generateChildren(int numChildren);
			void updateChildren();
			
			void assignAffector(BaseAffector newAffector);
			void removeAffector(std::string affectorName);
	};

}

#endif // PARTICLE_EMITTER_H
