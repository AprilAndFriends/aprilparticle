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
#include <gtypes/Vector3.h>

#include "aprilparticleExport.h"

namespace aprilparticle
{
	class Affector;
	class ParticleEmitter;

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

	class aprilparticleExport Particle
	{
	public:
		friend class Affector;
		friend class ParticleEmitter;
		friend class Affectors::Attractor;
		friend class Affectors::CallbackAffector;
		friend class Affectors::ColorChanger;
		friend class Affectors::DirectionalForceField;
		friend class Affectors::LinearDirectionalForce;
		friend class Affectors::MultiColorChanger;
		friend class Affectors::Rotator;
		friend class Affectors::Swirl;

		Particle();
		Particle(gvec3 position, gvec3 direction, float life, float scale, float speed = 0.0f, float angle = 0.0f, april::Color color = APRIL_COLOR_WHITE);
		virtual ~Particle();

		gvec3 getPosition() { return this->position; }
		void setPosition(gvec3 value) { this->position = value; }
		gvec3 getDirection() { return this->direction; }
		void setDirection(gvec3 value) { this->direction = value; }
		float getLife() { return this->life; }
		void setLife(float value) { this->life = value; }
		float getTotalLife() { return this->totalLife; }
		void setTotalLife(float value) { this->totalLife = value; }
		float getScale() { return this->scale; }
		void setScale(float value) { this->scale = value; }
		float getSpeed() { return this->speed; }
		void setSpeed(float value) { this->speed = value; }
		float getAngle() { return this->angle; }
		void setAngle(float value) { this->angle = value; }
		april::Color getColor() { return this->color; }
		void setColor(april::Color value) { this->color = value; }

		float getLifeProgressRatio();
		bool isDead();

	protected:
		gvec3 position;
		gvec3 direction;
		float life;
		float totalLife;
		float scale;
		float speed;
		float angle;
		april::Color color;

	};
}

#endif
