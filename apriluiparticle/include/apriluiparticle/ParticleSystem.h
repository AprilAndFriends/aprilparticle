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
/// Represents a GUI object that controls a particle system.

#ifndef APRILUIPARTICLE_PARTICLE_SYSTEM_H
#define APRILUIPARTICLE_PARTICLE_SYSTEM_H

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
	class ParticleSpaceBase;
	class ParticleSpace;

	class apriluiparticleExport ParticleSystem : public aprilui::Object
	{
	public:
		friend class ParticleSpaceBase;
		friend class ParticleSpace;

		ParticleSystem(chstr name, grect rect);
		~ParticleSystem();
		static aprilui::Object* createInstance(chstr name, grect rect);

		HL_DEFINE_GETSET(hstr, filename, Filename);
		HL_DEFINE_GET(aprilparticle::System*, system, System);
		bool isRunning();
		bool isExpired();
		
		void notifyEvent(chstr name, void* params);

		void load(chstr filename);
		void finishSystem();
		void stopSystem();
		void resetSystem();
		void OnDraw();
		
		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);
		
	protected:
		hstr filename;
		aprilparticle::System* system;
		harray<ParticleSpaceBase*> spaceBaseObjects;
		harray<ParticleSpace*> spaceObjects;

		void _createSpaceObject(aprilparticle::Space* space);
		void _destroySpaceObjects();

		void _registerSpaceObject(ParticleSpace* spaceObject);
		void _unregisterSpaceObject(ParticleSpace* spaceObject);
		void _assignSpaceObjectSpace(ParticleSpace* spaceObject, aprilparticle::Space* space);
		void _unassignSpaceObjectSpace(ParticleSpace* spaceObject, aprilparticle::Space* space);

		void _load();
		void _resize();

	};
}

#endif