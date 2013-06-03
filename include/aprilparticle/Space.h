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
/// Represents space with particles in it.

#ifndef APRILPARTICLE_SPACE_H
#define APRILPARTICLE_SPACE_H

#include <april/Color.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/hdeque.h>
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
		HL_DEFINE_GETSET(gvec3, up, Up);
		void setUp(float x, float y, float z) { this->up.set(x, y, z); }
		HL_DEFINE_GETSET(harray<Emitter*>, emitters, Emitters);
		bool isRunning();
		bool isExpired();
		int getParticleCount();

		void reset();
		void update(float k);
		void finish();
		void draw(gvec3 point); // is the only 3D drawing method
		void draw(gvec2 offset, april::Color color = april::Color::White);

		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);

		bool registerEmitter(Emitter* emitter);
		bool unregisterEmitter(Emitter* emitter);
		bool addAffector(Affector* affector);
		bool removeAffector(Affector* affector);
		Emitter* getEmitter(chstr name);

		void drawAffectors(); // usually only used for debug purposes, 3D

	protected:
		float preUpdate;
		gvec3 up;
		System* system;
		harray<Emitter*> emitters;
		hdeque<Particle*> particles;
		int alive;
		bool started;

		void _setSystem(System* value) { this->system = value; }

		void _addNewParticle(float k);

	private:
		gvec3 _movement;
		Particle* _particle;

	};
}

#endif
