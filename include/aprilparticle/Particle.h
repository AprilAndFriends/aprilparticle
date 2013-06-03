/// @file
/// @author  Domagoj Cerjan
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
/// Represents a particle.

#ifndef APRILPARTICLE_PARTICLE_H
#define APRILPARTICLE_PARTICLE_H

#include <april/Color.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>

#include "aprilparticleExport.h"

namespace aprilparticle
{
	class Affector;
	class Emitter;
	class Space;

	namespace Affectors
	{
		class Attractor;
		class CallbackAffector;
		class ColorChanger;
		class ColorChangerTimed;
		class Revolutor;
		class ForceField;
		class LinearForce;
		class Resizer;
		class ResizerTimed;
		class Rotator;
		class Scaler;
		class ScalerTimed;
	}

	class aprilparticleExport Particle
	{
	public:
		friend class Affector;
		friend class Emitter;
		friend class Space;
		friend class Affectors::Attractor;
		friend class Affectors::CallbackAffector;
		friend class Affectors::ColorChanger;
		friend class Affectors::ColorChangerTimed;
		friend class Affectors::Revolutor;
		friend class Affectors::ForceField;
		friend class Affectors::LinearForce;
		friend class Affectors::Resizer;
		friend class Affectors::ResizerTimed;
		friend class Affectors::Rotator;
		friend class Affectors::Scaler;
		friend class Affectors::ScalerTimed;

		Particle();
		virtual ~Particle();

		HL_DEFINE_GETSET(float, life, Life);
		HL_DEFINE_GETSET(gvec3, position, Position);
		HL_DEFINE_GETSET(gvec3, direction, Direction);
		HL_DEFINE_GETSET(gvec2, size, Size);
		HL_DEFINE_GETSET(float, scale, Scale);
		HL_DEFINE_GETSET(float, angle, Angle);
		HL_DEFINE_GETSET(april::Color, color, Color);

		float getLifeProgressRatio();
		bool isDead();

	protected:
		float timer;
		float life;
		gvec3 position;
		gvec3 direction;
		gvec2 size;
		float scale;
		float angle;
		april::Color color;

	};
}

#endif
