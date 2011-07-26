#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <april/RenderSystem.h>
#include <hltypes/hlist.h>
#include <hltypes/hmap.h>

#include "aprilparticleExport.h"
#include "CompositeParticleObject.h"

namespace april
{
	class CompositeParticleObject;
	class Texture;

	class aprilparticleExport ParticleSystem
	{
	public:
		ParticleSystem();
		~ParticleSystem();
		
		//CompositeParticleObject* getParticleObject(chstr name);
		CompositeParticleObject* loadParticleObject(chstr filename, chstr name = "");
		void update(double t);
		void draw();
		
	protected:
		hmap<april::Texture*, int> _mTextures;
		hlist<april::CompositeParticleObject*> _mComposites;
		int _mNumerator;
		
	};

}

#endif
