/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a whole particle system.

#ifndef APRILPARTICLE_SYSTEM_H
#define APRILPARTICLE_SYSTEM_H

#include <april/RenderSystem.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "aprilparticleExport.h"
#include "ActiveObject.h"

namespace april
{
	class Texture;
}

namespace hlxml
{
	struct Node;
}

namespace aprilparticle
{
	class Affector;
	class Emitter;

	class aprilparticleExport System : public ActiveObject
	{
	public:
		System(chstr filename = "", chstr name = "");
		~System();

		HL_DEFINE_GET(hstr, filename, Filename);
		
		bool registerEmitter(Emitter* emitter);
		bool unregisterEmitter(Emitter* emitter);
		bool registerAffector(Affector* affector);
		bool unregisterAffector(Affector* affector);
		bool registerTexture(april::Texture* texture, chstr name = "");
		bool unregisterTexture(chstr name);
		bool unregisterTexture(april::Texture* texture);
		Emitter* getEmitter(chstr name);
		april::Texture* getTexture(chstr name);
		
		void load();
		void update(float k);
		void draw(gvec3 point);
		
	protected:
		hstr filename;
		bool loaded;
		harray<Emitter*> emitters;
		hmap<hstr, april::Texture*> textures;

		void _loadEmitter(hlxml::Node* root);
		void _loadAffector(hlxml::Node* root, Emitter* emitter = NULL);
		void _loadTexture(hlxml::Node* root, Emitter* emitter = NULL);

	private:
		// TODO
		//gvec3 _position;
		hmap<Emitter*, harray<hstr>> _mappedAffectors;
		hmap<Emitter*, hstr> _mappedTextures;
		
	};
}

#endif
