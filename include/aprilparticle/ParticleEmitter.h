#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "aprilparticleExport.h"
#include "april/RenderSystem.h"
#include "BaseParticle.h"
#include "Affectors.h" // includes all affectors
#include <string>
#include <list>

namespace April
{

	class aprilparticleExport ParticleEmitter : public BaseParticle
	{
		public:
		
			std::list<ParticleEmitter> particles;
			std::list<BaseAffector*> affecttors; 
			
			bool randomSize;			// used to define weather to use random size on children particles
			float sizemin, sizemax;		// if randomSize is true use this as a size range
			
			float creationSpeed;
			float speed;
			float counter;
			
			unsigned int maxParticles;	// max number of children alive
			float particleDecaySpeed;	// decay speed of children
			
			April::Texture* tex;
		
			ParticleEmitter();
			ParticleEmitter(float life, float size, float creationSpeed = 0.1, float speed = 0.1, gvec3 pos = gvec3(0,0,0), gvec3 direction = gvec3(0,0,1),
							bool rand = false, float smin = 0, float smax = 1, unsigned int max = 256, float decay = 1.0);
			~ParticleEmitter();
			
			void setTexture(std::string texture);
			void addAffctor(BaseAffector *affector);
			void update(float t);
			void draw();
			
	};

}

#endif // PARTICLE_EMITTER_H
