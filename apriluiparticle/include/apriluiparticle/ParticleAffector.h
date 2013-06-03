/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a GUI object that renders particles.

#ifndef APRILUIPARTICLE_PARTICLE_AFFECTOR_H
#define APRILUIPARTICLE_PARTICLE_AFFECTOR_H

#include <aprilui/Object.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "apriluiparticleExport.h"

namespace apriluiparticle
{
	class ParticleEmitter;

	class apriluiparticleExport ParticleAffector : public aprilui::Object
	{
	public:
		ParticleAffector(chstr name, grect rect);
		~ParticleAffector();
		static aprilui::Object* createInstance(chstr name, grect rect);

		HL_DEFINE_GETSET(hstr, emitterName, EmitterName);
		
		void notifyEvent(chstr name, void* params);

		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);
		
		void update(float k);
		void OnDraw();
		
	protected:
		hstr emitterName;
		ParticleEmitter* emitter;

	};
}

#endif