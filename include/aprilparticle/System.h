/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.6
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

#include <april/Color.h>
#include <april/RenderSystem.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "aprilparticleExport.h"
#include "ActiveObject.h"

namespace april
{
	class Texture;
}

namespace hlxml
{
	class Node;
}

namespace aprilparticle
{
	class Affector;
	class Emitter;
	class Texture;

	class aprilparticleExport System : public ActiveObject
	{
	public:
		System(chstr filename = "", chstr name = "");
		System(const System& other);
		~System();

		bool isRunning();
		HL_DEFINE_GET(hstr, filename, Filename);
		HL_DEFINE_GETSET(gvec3, up, Up);
		void setUp(float x, float y, float z) { this->up.set(x, y, z); }
		HL_DEFINE_GET(harray<Emitter*>, emitters, Emitters);
		
		bool registerEmitter(Emitter* emitter);
		bool unregisterEmitter(Emitter* emitter);
		bool registerAffector(Affector* affector);
		bool unregisterAffector(Affector* affector);
		void addAffector(Affector* affector);
		void removeAffector(Affector* affector);
		bool registerTexture(aprilparticle::Texture* texture, chstr name = "");
		bool unregisterTexture(chstr name);
		bool unregisterTexture(aprilparticle::Texture* texture);
		Emitter* getEmitter(chstr name);
		aprilparticle::Texture* getTexture(chstr name);
		int getParticleCount();
		bool isExpired();
		
		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);

		void reset();
		void load();
		void update(float k);
		void finish();
		void draw(gvec3 point); // is the only 3D drawing method
		void draw(gvec2 offset = gvec2(), april::Color color = APRIL_COLOR_WHITE);
		
	protected:
		hstr filename;
		gvec3 up;
		bool loaded;
		harray<Emitter*> emitters;
		hmap<hstr, aprilparticle::Texture*> textures;

		void _loadEmitter(hlxml::Node* root);
		void _loadAffector(hlxml::Node* root, Emitter* emitter = NULL);
		void _loadTexture(hlxml::Node* root, Emitter* emitter = NULL);

		void _assignEmitterData();

	private:
		hmap<Emitter*, harray<hstr> > _mappedAffectors;
		hmap<Emitter*, hstr> _mappedTextures;
		
	};
}

#endif
