#ifndef COMPOSITE_PARTICLE_OBJECT_H
#define COMPOSITE_PARTICLE_OBJECT_H

#include <gtypes/Vector3.h>
#include <hltypes/hlist.h>

#include "aprilparticleExport.h"

namespace april
{
	class ParticleEmitter;
	class Texture;

	namespace Affectors
	{
		class Affector;
	}

	class aprilparticleExport CompositeParticleObject
	{
	public:
		CompositeParticleObject();
		~CompositeParticleObject();
		
		void addEmitter(april::ParticleEmitter* emitter);
		void addAffector(april::Affectors::Affector* affector);
		void addTexture(april::Texture* texture);

	protected:
		gvec3 _position;
		hlist<april::ParticleEmitter*> _emitters;
		hlist<april::Affectors::Affector*> _affectors;
		hlist<april::Texture*> _textures;

	};

}

#endif
