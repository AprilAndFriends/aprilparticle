/// @file
/// @author  Boris Mikic
/// @version 1.63
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a GUI object that renders particles.

#ifndef APRILUIPARTICLE_PARTICLE_H
#define APRILUIPARTICLE_PARTICLE_H

#include <aprilui/Object.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "apriluiparticleExport.h"

namespace aprilparticle
{
	class System;
}

namespace apriluiparticle
{
	class apriluiparticleExport Particle : public aprilui::Object
	{
	public:
		Particle(chstr name, grect rect);
		~Particle();
		static aprilui::Object* createInstance(chstr name, grect rect);

		HL_DEFINE_GETSET(hstr, filename, Filename);
		HL_DEFINE_ISSET(bool, globalSpace, GlobalSpace);
		HL_DEFINE_GET(aprilparticle::System*, system, System);
		bool isRunning();
		bool isExpired();
		
		void notifyEvent(chstr name, void* params);

		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);
		
		void load(chstr filename);
		void update(float k);
		void finishSystem();
		void stopSystem();
		void resetSystem();
		void OnDraw();
		
	protected:
		hstr filename;
		bool globalSpace;
		gvec2 initialPosition;
		gvec3 systemPosition;
		aprilparticle::System* system;

		void _loadParticleSystem();
		void _resizeSystem();

	};
}

#endif