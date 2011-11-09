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

#ifndef APRILPARTICLE_SYSTEM_H
#define APRILPARTICLE_SYSTEM_H

#include <april/RenderSystem.h>
#include <gtypes/Vector3.h>
#include <hltypes/hlist.h>
#include <hltypes/hmap.h>
#include <hltypes/util.h>

#include "aprilparticleExport.h"
#include "Space3DObject.h"

namespace april
{
	class Texture;
}

namespace aprilparticle
{
	class Affector;
	class Emitter;

	class aprilparticleExport System
	{
	public:
		System(chstr filename = "", chstr name = "");
		~System();

		HL_DEFINE_GET(hstr, filename, Filename);
		HL_DEFINE_GETSET(hstr, name, Name);
		HL_DEFINE_GETSET(gvec3, position, Position);
		void setPosition(float x, float y, float z) { this->position.set(x, y, z); }
		/*// TODO - delete?
		float getX() { return this->position.x; }
		void setX(float value) { this->position.x = value; }
		float getY() { return this->position.y; }
		void setY(float value) { this->position.y = value; }
		float getZ() { return this->position.z; }
		void setZ(float value) { this->position.z = value; }
		*/
		
		void registerEmitter(Emitter* emitter);
		void unregisterEmitter(Emitter* emitter);
		void registerAffector(Affector* affector);
		void unregisterAffector(Affector* affector);
		void registerTexture(april::Texture* texture);
		void unregisterTexture(april::Texture* texture);

		void load();
		void update(float k);
		void draw(gvec3 point, gvec3 up);
		
	protected:
		hstr filename;
		hstr name;
		gvec3 position;
		harray<Emitter*> emitters;
		harray<Affector*> affectors;
		harray<april::Texture*> textures;

	private:
		gvec3 _position;
		hmap<april::Texture*, int> _textures;
		
	};
}

#endif
