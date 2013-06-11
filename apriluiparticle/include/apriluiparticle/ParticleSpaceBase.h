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

#ifndef APRILUIPARTICLE_PARTICLE_SPACE_BASE_H
#define APRILUIPARTICLE_PARTICLE_SPACE_BASE_H

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
}

namespace apriluiparticle
{
	class ParticleSystem;

	class apriluiparticleExport ParticleSpaceBase : public aprilui::Object
	{
	public:
		friend class ParticleSystem;

		ParticleSpaceBase(chstr name, grect rect);
		~ParticleSpaceBase();
		static aprilui::Object* createInstance(chstr name, grect rect);

		void update(float k);
		void OnDraw();
		
	protected:
		ParticleSystem* systemObject;
		aprilparticle::Space* space;

		void _setSystemSpace(ParticleSystem* systemObject, aprilparticle::Space* space);

		void _reset();

	private:
		bool _firstFrame; // required due to a system incompatibility between AprilUI and AprilParticle

	};
}

#endif