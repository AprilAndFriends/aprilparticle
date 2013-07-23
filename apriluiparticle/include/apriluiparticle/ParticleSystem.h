/// @file
/// @author  Boris Mikic
/// @version 2.01
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
#include "ParticleBase.h"

namespace aprilparticle
{
	class Space;
}

namespace apriluiparticle
{
	class ParticleSpace;

	class apriluiparticleExport ParticleSystem : public ParticleBase
	{
	public:
		friend class ParticleSpace;

		ParticleSystem(chstr name, grect rect);
		~ParticleSystem();
		static aprilui::Object* createInstance(chstr name, grect rect);

		void stopSystem();
		void resetSystem();

	protected:
		harray<ParticleSpace*> spaceObjects;

		void _registerSpaceObject(ParticleSpace* spaceObject);
		void _unregisterSpaceObject(ParticleSpace* spaceObject);

	};
}

#endif