/// @file
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
/// Represents an active object.

#ifndef APRILPARTICLE_ACTIVE_OBJECT_H
#define APRILPARTICLE_ACTIVE_OBJECT_H

#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "aprilparticleExport.h"
#include "Space3DObject.h"

namespace aprilparticle
{
	class Affector;

	class aprilparticleExport ActiveObject : public Space3DObject
	{
	public:
		ActiveObject(chstr name = "");
		~ActiveObject();

		HL_DEFINE_GETSET(hstr, name, Name);
		HL_DEFINE_ISSET(bool, visible, Visible);
		HL_DEFINE_ISSET(bool, enabled, Enabled);

		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);

		Affector* getAffector(chstr name);
		template <class T>
		T* getAffector(chstr name)
		{
			return dynamic_cast<T*>(this->getAffector(name));
		}

	protected:
		hstr name;
		bool visible;
		bool enabled;
		harray<Affector*> affectors;

	};
}

#endif
