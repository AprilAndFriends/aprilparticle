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
#include "Base.h"

namespace apriluiparticle
{
	class apriluiparticleExport Particle : public Base
	{
		APRILUI_CLONEABLE(Particle);
	public:
		Particle(chstr name);
		~Particle();
		inline hstr getClassName() const { return "apriluiparticle.Particle"; }

		static aprilui::Object* createInstance(chstr name);

		void update(float timeDelta);

	protected:
		void _draw();

	};
}

#endif