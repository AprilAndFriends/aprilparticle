/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Represents space with particles in it.

#ifndef APRILPARTICLE_SPACE_H
#define APRILPARTICLE_SPACE_H

#include <april/Color.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>

#include "SpaceObject.h"
#include "aprilparticleExport.h"
#include "AffectorContainer.h"

namespace aprilparticle
{
	class Emitter;
	class Particle;
	class System;

	class aprilparticleExport Space : public SpaceObject, public AffectorContainer
	{
	public:
		friend class Emitter;
		friend class System;

		Space(chstr name = "");
		Space(const Space& other);
		~Space();

		HL_DEFINE_GETSET(float, preUpdate, PreUpdate);
		HL_DEFINE_GETSET(float, fixedTimeStep, FixedTimeStep);
		HL_DEFINE_GETSET(gvec3, up, Up);
		inline void setUp(float x, float y, float z) { this->up.set(x, y, z); }
		HL_DEFINE_GETSET(harray<Emitter*>, emitters, Emitters);
		bool isRunning();
		bool isExpired();
		int getParticleCount();

		harray<PropertyDescription> getPropertyDescriptions();

		void reset();
		void update(float timeDelta);
		void finish();
		void draw(gvec3 point); // is the only 3D drawing method
		void draw(gvec2 offset, april::Color color = april::Color::White);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		bool registerEmitter(Emitter* emitter);
		bool unregisterEmitter(Emitter* emitter);
		bool addAffector(Affector* affector);
		bool removeAffector(Affector* affector);
		Emitter* getEmitter(chstr name);

		void drawAffectors(); // usually only used for debug purposes, 3D

	protected:
		float preUpdate;
		float fixedTimeStep;
		gvec3 up;
		System* system;
		harray<Emitter*> emitters;
		harray<Particle*> particles;
		int alive;
		bool started;

		inline void _setSystem(System* value) { this->system = value; }

		void _updateInternal(float timeDelta);

		gvec3 _addNewParticle(float timeDelta);

	private:
		static harray<PropertyDescription> _propertyDescriptions;

		gvec3 _movement;
		gvec3 _movementDirection;
		gvec3 _initialDirection;
		Particle* _particle;
		float _lastTimeFraction;

	};
}

#endif
