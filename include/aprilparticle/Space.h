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

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GETSET(float, preUpdate, PreUpdate);
		HL_DEFINE_GETSET(float, fixedTimeStep, FixedTimeStep);
		HL_DEFINE_GETSET(gvec3f, up, Up);
		inline void setUp(float x, float y, float z) { this->up.set(x, y, z); }
		HL_DEFINE_GETSET(harray<Emitter*>, emitters, Emitters);
		bool isRunning() const;
		bool isExpired() const;
		int getParticleCount() const;

		void reset();
		void update(float timeDelta);
		void finish();
		void draw(cgvec3f point); // is the only 3D drawing method
		void draw(cgvec2f offset, const april::Color& color = april::Color::White);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		bool registerEmitter(Emitter* emitter);
		bool unregisterEmitter(Emitter* emitter);
		bool addAffector(Affector* affector);
		bool removeAffector(Affector* affector);
		Emitter* getEmitter(chstr name) const;

		void drawAffectors(); // usually only used for debug purposes, 3D

	protected:
		float preUpdate;
		float fixedTimeStep;
		gvec3f up;
		System* system;
		harray<Emitter*> emitters;
		harray<Particle*> particles;
		int alive;
		bool started;

		inline void _setSystem(System* value) { this->system = value; }

		void _updateInternal(float timeDelta);

		gvec3f _addNewParticle(float timeDelta);

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;

		gvec3f _movement;
		gvec3f _movementDirection;
		gvec3f _initialDirection;
		Particle* _particle;
		float _lastTimeFraction;

	};
}

#endif
