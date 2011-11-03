/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a whole particle system.

#ifndef APRILPARTICLE_PARTICLE_SYSTEM_H
#define APRILPARTICLE_PARTICLE_SYSTEM_H

#include <april/RenderSystem.h>
#include <hltypes/hlist.h>
#include <hltypes/hmap.h>

#include "aprilparticleExport.h"

namespace april
{
	class Texture;
}

namespace aprilparticle
{
	class Affector;
	class ParticleEmitter;

	class aprilparticleExport ParticleSystem
	{
	public:
		ParticleSystem(chstr filename, chstr name = "");
		~ParticleSystem();
		
		void load();
		void update(float k);
		void draw();
		
		void addEmitter(ParticleEmitter* emitter);
		void addAffector(Affector* affector);
		void addTexture(april::Texture* texture);

	protected:
		hstr filename;
		hstr name;

		gvec3 _position;
		hlist<ParticleEmitter*> _emitters;
		hlist<Affector*> _affectors;
		hlist<april::Texture*> _textures;

		hmap<april::Texture*, int> textures;
		
	};

}

#endif
