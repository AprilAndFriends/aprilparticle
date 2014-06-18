/// @file
/// @version 2.11
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
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
		hstr getClassName() const { return "ParticleBase"; }

		static aprilui::Object* createInstance(chstr name, grect rect);

		HL_DEFINE_GETSET(hstr, filename, Filename);
		HL_DEFINE_GETSET(hstr, filepath, Filepath);
		HL_DEFINE_GET(aprilparticle::System*, system, System);
		bool isRunning();
		bool isExpired();
		
		harray<aprilui::PropertyDescription> getPropertyDescriptions();

		void notifyEvent(chstr name, void* params);

		void load(chstr filename);
		void finishSystem();
		virtual void stopSystem();
		virtual void resetSystem();
		
		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);
		
	protected:
		hstr filename;
		hstr filepath;
		aprilparticle::System* system;

		inline aprilparticle::System** _getSystemPtr() { return &this->system; }

		virtual void _load();
		virtual void _resize();

	private:
		static harray<aprilui::PropertyDescription> _propertyDescriptions;

	};
}

#endif