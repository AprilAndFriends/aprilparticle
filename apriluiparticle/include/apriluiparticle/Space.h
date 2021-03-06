/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Represents a indepdent GUI object that renders particles.

#ifndef APRILUIPARTICLE_PARTICLE_SPACE_H
#define APRILUIPARTICLE_PARTICLE_SPACE_H

#include <aprilui/Object.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "apriluiparticleExport.h"

namespace aprilparticle
{
	class Space;
	class System;
}

namespace apriluiparticle
{
	class Emitter;
	class System;

	class apriluiparticleExport Space : public aprilui::Object
	{
		APRILUI_CLONEABLE(Space);
	public:
		friend class Emitter;
		friend class System;

		Space(chstr name);
		~Space();
		inline hstr getClassName() const { return "apriluiparticle.Space"; }
		static aprilui::Object* createInstance(chstr name);

		hmap<hstr, aprilui::PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GET(System*, systemObject, SystemObject);
		HL_DEFINE_GET(harray<Emitter*>, emitterObjects, EmitterObjects);
		HL_DEFINE_GET(aprilparticle::Space*, space, Space);
		HL_DEFINE_GETSET(hstr, systemObjectName, SystemObjectName);
		HL_DEFINE_GETSET(hstr, spaceName, SpaceName);

		void notifyEvent(chstr type, aprilui::EventArgs* args);

	protected:
		System* systemObject;
		harray<Emitter*> emitterObjects;
		aprilparticle::Space* space;
		hstr systemObjectName;
		hstr spaceName;

		hmap<hstr, aprilui::PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, aprilui::PropertyDescription::Accessor*>& _getSetters() const;

		void _update(float timeDelta);
		void _draw();

		void _updateBindings();
		void _tryFindSystemObject();
		void _tryFindSpace();
		void _registerEmitterObject(Emitter* emitter);
		void _unregisterEmitterObject(Emitter* emitter);
		void _unbind();
		void _resize();

	private:
		static hmap<hstr, aprilui::PropertyDescription> _propertyDescriptions;
		static hmap<hstr, aprilui::PropertyDescription::Accessor*> _getters;
		static hmap<hstr, aprilui::PropertyDescription::Accessor*> _setters;

	};

}
#endif