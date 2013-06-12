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

#ifndef APRILUIPARTICLE_PARTICLE_H
#define APRILUIPARTICLE_PARTICLE_H

#include <aprilui/Object.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "apriluiparticleExport.h"
#include "ParticleBase.h"

namespace apriluiparticle
{
	class apriluiparticleExport Particle : public ParticleBase
	{
	public:
		Particle(chstr name, grect rect);
		~Particle();
		static aprilui::Object* createInstance(chstr name, grect rect);

		void update(float k);

		void OnDraw();

	};
}

#endif