#ifndef COMPOSITE_PARTICLE_OBJECT_H
#define COMPOSITE_PARTICLE_OBJECT_H

#include <gtypes/Vector3.h>
#include <hltypes/hlist.h>

#include "aprilparticleExport.h"

namespace april
{
	class Texture;
}

namespace aprilparticle
{
	class Affector;
	class ParticleEmitter;

	class aprilparticleExport CompositeParticleObject
	{
	public:
		CompositeParticleObject();
		~CompositeParticleObject();
		
		void addEmitter(ParticleEmitter* emitter);
		void addAffector(Affector* affector);
		void addTexture(april::Texture* texture);

	protected:
		gvec3 _position;
		hlist<ParticleEmitter*> _emitters;
		hlist<Affector*> _affectors;
		hlist<april::Texture*> _textures;

	};

}

#endif
