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
/// Represents a GUI object that controls a particle system.

#ifndef APRILUIPARTICLE_BASE_H
#define APRILUIPARTICLE_BASE_H

#include <aprilui/PropertyDescription.h>
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
	class Space;

	class apriluiparticleExport Base : public aprilui::Object
	{
		APRILUI_CLONEABLE(Base);
	public:
		friend class Space;

		Base(chstr name);
		~Base();
		inline hstr getClassName() const { return "apriluiparticle.Base"; }

		hmap<hstr, aprilui::PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_GETSET(hstr, filename, Filename);
		HL_DEFINE_GETSET(hstr, filepath, Filepath);
		HL_DEFINE_IS(loaded, Loaded);
		HL_DEFINE_ISSET(alwaysEnabled, AlwaysEnabled);
		HL_DEFINE_GET(aprilparticle::System*, system, System);
		bool isRunning() const;
		bool isExpired() const;
		
		void notifyEvent(chstr type, aprilui::EventArgs* args);

		void load(chstr filename);
		void finishSystem();
		virtual void stopSystem();
		virtual void resetSystem();
		
		bool setProperty(chstr name, chstr value);
		
	protected:
		hstr filename;
		hstr filepath;
		bool loaded;
		bool alwaysEnabled;
		aprilparticle::System* system;

		inline aprilparticle::System** _getSystemPtr() { return &this->system; }

		hmap<hstr, aprilui::PropertyDescription::Accessor*>& _getGetters() const;
		hmap<hstr, aprilui::PropertyDescription::Accessor*>& _getSetters() const;

		virtual void _load();
		virtual void _resize();

	private:
		static hmap<hstr, aprilui::PropertyDescription> _propertyDescriptions;
		static hmap<hstr, aprilui::PropertyDescription::Accessor*> _getters;
		static hmap<hstr, aprilui::PropertyDescription::Accessor*> _setters;

	};

}
#endif