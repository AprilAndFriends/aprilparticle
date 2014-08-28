/// @file
/// @version 2.11
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
	class ParticleEmitter;
	class ParticleSystem;

	class apriluiparticleExport ParticleSpace : public aprilui::Object
	{
	public:
		friend class ParticleEmitter;
		friend class ParticleSystem;

		ParticleSpace(chstr name, grect rect);
		~ParticleSpace();
		hstr getClassName() const { return "ParticleSpace"; }

		static aprilui::Object* createInstance(chstr name, grect rect);

		HL_DEFINE_GETSET(hstr, systemObjectName, SystemObjectName);
		HL_DEFINE_GETSET(hstr, spaceName, SpaceName);

		harray<aprilui::PropertyDescription> getPropertyDescriptions();

		void notifyEvent(aprilui::Event::Type type, aprilui::EventArgs* args);

		void update(float timeDelta);
		void OnDraw();
		
		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);
		
	protected:
		ParticleSystem* systemObject;
		aprilparticle::Space* space;
		hstr systemObjectName;
		hstr spaceName;
		harray<ParticleEmitter*> emitterObjects;

		void _updateBindings();
		void _tryFindSystemObject();
		void _tryFindSpace();
		void _registerEmitterObject(ParticleEmitter* emitter);
		void _unregisterEmitterObject(ParticleEmitter* emitter);
		void _unbind();
		void _resize();

	private:
		static harray<aprilui::PropertyDescription> _propertyDescriptions;

	};
}

#endif