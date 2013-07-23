/// @file
/// @author  Boris Mikic
/// @version 2.01
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a GUI object that controls a particle system.

#ifndef APRILUIPARTICLE_PARTICLE_BASE_H
#define APRILUIPARTICLE_PARTICLE_BASE_H

#include <aprilui/Object.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "apriluiparticleExport.h"

namespace aprilparticle
{
	class Space;
	class System;
}

namespace apriluiparticle
{
	class ParticleSpaceBase;
	class ParticleSpace;

	class apriluiparticleExport ParticleBase : public aprilui::Object
	{
	public:
		friend class ParticleSpaceBase;
		friend class ParticleSpace;

		ParticleBase(chstr name, grect rect);
		~ParticleBase();
		static aprilui::Object* createInstance(chstr name, grect rect);

		HL_DEFINE_GETSET(hstr, filename, Filename);
		HL_DEFINE_GETSET(hstr, filepath, Filepath);
		HL_DEFINE_GET(aprilparticle::System*, system, System);
		bool isRunning();
		bool isExpired();
		
		void notifyEvent(chstr name, void* params);

		void load(chstr filename);
		void finishSystem();
		virtual void stopSystem();
		virtual void resetSystem();
		
		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);
		
	protected:
		hstr filename;
		hstr filepath;
		aprilparticle::System* system;

		aprilparticle::System** _getSystemPtr() { return &this->system; }

		virtual void _load();
		virtual void _resize();

	};
}

#endif