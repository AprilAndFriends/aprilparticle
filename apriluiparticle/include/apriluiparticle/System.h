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
/// Represents a GUI object that controls a particle system.

#ifndef APRILUIPARTICLE_SYSTEM_H
#define APRILUIPARTICLE_SYSTEM_H

#include <aprilui/Object.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "apriluiparticleExport.h"
#include "Base.h"

namespace aprilparticle
{
	class Space;
}

namespace apriluiparticle
{
	class Emitter;
	class Space;

	class apriluiparticleExport System : public Base
	{
		APRILUI_CLONEABLE(System);
	public:
		friend class Space;

		System(chstr name);
		~System();
		inline hstr getClassName() const { return "apriluiparticle.System"; }
		static aprilui::Object* createInstance(chstr name);

		HL_DEFINE_GET(harray<Space*>, spaceObjects, SpaceObjects);
		harray<Emitter*> getEmitterObjects() const;

		void stopSystem();
		void resetSystem();

	protected:
		bool previousExpired;
		harray<Space*> spaceObjects;

		void _registerSpaceObject(Space* spaceObject);
		void _unregisterSpaceObject(Space* spaceObject);

		void _load();

		void _update(float timeDelta);

	};
}

#endif