#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "aprilparticleExport.h"
#include "april/RenderSystem.h"
#include "BaseParticle.h"
#include <string>
#include <list>


namespace April
{

	class aprilparticleExport ParticleEmitter : public BaseParticle
	{
		public:
		
			std::list<ParticleEmitter> particles;
			
			bool randomSize;			// used to define weather to use random size on children particles
			float sizemin, sizemax;		// if randomSize is true use this as a size range
			
			float creationSpeed;
			float speed;
			float counter;
			
			unsigned int maxParticles;	// max number of children alive
			float particleDecaySpeed;	// decay speed of children
			
			April::Texture* tex;
		
		
		
			ParticleEmitter();
			ParticleEmitter(float life, float size, float creationSpeed = 0.1, float speed = 0.1, bool rand = false, float smin = 0, float smax = 1, unsigned int max = 64, float decay = 1.0);
			~ParticleEmitter();
			
			void setTexture(std::string texture);
			void update(float t);
			void draw();
			
	};

}

#endif // PARTICLE_EMITTER_H
