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
/// Represents a indepdent GUI object that renders particles.

#ifndef APRILUIPARTICLE_PARTICLE_SPACE_H
#define APRILUIPARTICLE_PARTICLE_SPACE_H

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "apriluiparticleExport.h"
#include "ParticleSpaceBase.h"

namespace apriluiparticle
{
	class ParticleEmitter;
	class ParticleSystem;

	class apriluiparticleExport ParticleSpace : public ParticleSpaceBase
	{
	public:
		friend class ParticleEmitter;
		friend class ParticleSystem;

		ParticleSpace(chstr name, grect rect);
		~ParticleSpace();
		static aprilui::Object* createInstance(chstr name, grect rect);

		HL_DEFINE_GETSET(hstr, systemObjectName, SystemObjectName);
		HL_DEFINE_GETSET(hstr, spaceName, SpaceName);

		void notifyEvent(chstr name, void* params);

		void update(float k);
		void OnDraw();
		
		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);
		
	protected:
		hstr systemObjectName;
		hstr spaceName;

		void _tryFindSystemObject();
		void _tryFindSpace();
		void _resize();

	};
}

#endif