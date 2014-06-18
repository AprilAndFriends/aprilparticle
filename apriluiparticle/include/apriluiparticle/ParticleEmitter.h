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
	class ParticleSpace;

	class apriluiparticleExport ParticleEmitter : public aprilui::Object
	{
	public:
		friend class ParticleSpace;

		ParticleEmitter(chstr name, grect rect);
		~ParticleEmitter();
		hstr getClassName() const { return "ParticleEmitter"; }

		static aprilui::Object* createInstance(chstr name, grect rect);

		HL_DEFINE_GETSET(hstr, spaceObjectName, SpaceObjectName);
		HL_DEFINE_GETSET(hstr, emitterName, EmitterName);

		harray<aprilui::PropertyDescription> getPropertyDescriptions();

		void resetEmitter();

		void update(float timeDelta);
		void OnDraw();
		
		void notifyEvent(chstr name, void* params);
		
		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);
		
	protected:
		ParticleSpace* spaceObject;
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