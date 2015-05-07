/// @file
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Represents an independent GUI object that controls a particle emitter.

#ifndef APRILUIPARTICLE_PARTICLE_EMITTER_H
#define APRILUIPARTICLE_PARTICLE_EMITTER_H

#include <aprilui/Object.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "apriluiparticleExport.h"

namespace aprilparticle
{
	class Emitter;
	class System;
}

namespace apriluiparticle
{
	class Space;

	class apriluiparticleExport Emitter : public aprilui::Object
	{
		APRILUI_CLONEABLE(Emitter);
	public:
		friend class Space;

		Emitter(chstr name);
		~Emitter();
		inline hstr getClassName() const { return "apriluiparticle.Emitter"; }

		static aprilui::Object* createInstance(chstr name);

		HL_DEFINE_GETSET(hstr, spaceObjectName, SpaceObjectName);
		HL_DEFINE_GETSET(hstr, emitterName, EmitterName);

		harray<aprilui::PropertyDescription> getPropertyDescriptions();

		void resetEmitter();

		void notifyEvent(chstr type, aprilui::EventArgs* args);
		
		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);
		
	protected:
		Space* spaceObject;
		aprilparticle::Emitter* emitter;
		hstr spaceObjectName;
		hstr emitterName;
		gvec2 initialPosition;
		float initialAngle;
		gvec3 emitterPosition;
		gvec3 emitterMinDirection;
		gvec3 emitterMaxDirection;
		gvec2 emitterMinSize;
		gvec2 emitterMaxSize;

		void _update(float timeDelta);
		void _draw();

		void _updateEmitterData();
		void _updateBindings();
		void _tryFindSpaceObject();
		void _tryFindEmitter();
		void _unbind();

	private:
		static harray<aprilui::PropertyDescription> _propertyDescriptions;

	};
}

#endif