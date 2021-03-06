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

		HL_DEFINE_GETSET(float, life, Life);
		HL_DEFINE_GETSET(gvec3f, position, Position);
		HL_DEFINE_GETSET(gvec3f, direction, Direction);
		HL_DEFINE_GETSET(gvec2f, size, Size);
		HL_DEFINE_GETSET(float, scale, Scale);
		HL_DEFINE_GETSET(float, angle, Angle);
		HL_DEFINE_ISSET(directionAligned, DirectionAligned);
		HL_DEFINE_GETSET(april::Color, color, Color);

		float getLifeProgressRatio() const;
		bool isDead() const;

	protected:
		float timer;
		float life;
		gvec3f position;
		gvec3f direction;
		gvec2f size;
		float scale;
		float angle;
		bool directionAligned;
		april::Color color;

	};
}

#endif
