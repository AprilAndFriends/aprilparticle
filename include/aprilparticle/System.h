/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
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

#include "ActiveObject.h"
#include "AffectorContainer.h"
#include "aprilparticleExport.h"

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
	class Space;
	class Texture;

	class aprilparticleExport System : public ActiveObject, public AffectorContainer
	{
	public:
		System(chstr filename = "", chstr name = "");
		System(const System& other);
		~System();

		HL_DEFINE_GET(hstr, filename, Filename);
		HL_DEFINE_GET(harray<Space*>, spaces, Spaces);
		HL_DEFINE_GET2(hmap, hstr, Texture*, textures, Textures);
		harray<Emitter*> getEmitters() const;
		bool isRunning() const;
		bool isExpired() const;
		
		bool registerSpace(Space* space);
		bool unregisterSpace(Space* space);
		bool registerAffector(Affector* affector);
		bool unregisterAffector(Affector* affector);
		bool registerTexture(Texture* texture, chstr name = "");
		bool unregisterTexture(chstr name);
		bool unregisterTexture(Texture* texture);
		Space* getSpace(chstr name) const;
		Texture* getTexture(chstr name) const;
		Emitter* getEmitter(chstr name) const;
		int getParticleCount() const;

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;
		
		void reset();
		void load();
		void save(chstr filename);
		void update(float timeDelta);
		void finish();
		void draw(cgvec3f point); // is the only 3D drawing method
		void draw(cgvec2f offset = gvec2f(), const april::Color& color = april::Color::White);
		
	protected:
		hstr filename;
		bool loaded;
		harray<Space*> spaces;
		hmap<hstr, Texture*> textures;

		void _loadSpace(hlxml::Node* root);
		void _loadEmitter(hlxml::Node* root, Space* space);
		void _loadAffector(hlxml::Node* root, Space* space = NULL);
		void _loadTexture(hlxml::Node* root, Emitter* emitter = NULL);

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;
		
	};
}

#endif
