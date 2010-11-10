#ifndef _COMPOSITE_PARTICLE_OBJECT_H_
#define _COMPOSITE_PARTICLE_OBJECT_H_

#include "aprilparticleExport.h"
#include "April/RenderSystem.h"
#include "ParticleEmitter.h"
#include "Affectors.h"
#include "gtypes/Vector3.h"
#include "gtypes/Matrix4.h"
#include <list>

namespace April
{

	class aprilparticleExport CompositeParticleObject {
		
		gvec3 _mPosition;
		std::list<April::ParticleEmitter*> _mEmitters;
		std::list<April::Affectors::Affector*> _mAffectors;
		std::list<April::Texture *> _mTextures;

	public:
		CompositeParticleObject();
		~CompositeParticleObject();
		
		void addEmitter(April::ParticleEmitter *emitter);
		void addAffector(April::Affectors::Affector *affector);
		void addTexture(April::Texture *tex);

	};

}

#endif // _COMPOSITE_PARTICLE_OBJECT_H_
