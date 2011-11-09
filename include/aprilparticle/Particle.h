/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
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
#include <hltypes/util.h>

#include "aprilparticleExport.h"
#include "Space3DObject.h"

namespace aprilparticle
{
	class Affector;
	class Emitter;

	namespace Affectors
	{
		class Attractor;
		class CallbackAffector;
		class ColorChanger;
		class DirectionalForceField;
		class LinearDirectionalForce;
		class MultiColorChanger;
		class Rotator;
		class Swirl;
	}

	class aprilparticleExport Particle : public Space3DObject
	{
	public:
		friend class Affector;
		friend class Emitter;
		friend class Affectors::Attractor;
		friend class Affectors::CallbackAffector;
		friend class Affectors::ColorChanger;
		friend class Affectors::DirectionalForceField;
		friend class Affectors::LinearDirectionalForce;
		friend class Affectors::MultiColorChanger;
		friend class Affectors::Rotator;
		friend class Affectors::Swirl;

		Particle();
		Particle(gvec3 position, gvec3 direction = gvec3(0.0f, 1.0f, 0.0f), float life = 1.0f, gvec2 size = gvec2(1.0f, 1.0f), float scale = 1.0f, float speed = 0.0f, float angle = 0.0f, april::Color color = APRIL_COLOR_WHITE);
		virtual ~Particle();

		HL_DEFINE_GETSET(float, life, Life);
		HL_DEFINE_GETSET(gvec2, size, Size);
		HL_DEFINE_GETSET(float, scale, Scale);
		HL_DEFINE_GETSET(float, speed, Speed);
		HL_DEFINE_GETSET(float, angle, Angle);
		HL_DEFINE_GETSET(april::Color, color, Color);

		float getLifeProgressRatio();
		bool isDead();

	protected:
		float timer;
		float life;
		gvec2 size;
		float scale;
		float speed;
		float angle;
		april::Color color;

	};
}

#endif
