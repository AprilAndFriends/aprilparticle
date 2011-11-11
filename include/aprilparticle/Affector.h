/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a generic affector class.

#ifndef APRILPARTICLE_AFFECTOR_H
#define APRILPARTICLE_AFFECTOR_H

#include <hltypes/hstring.h>
#include <hltypes/util.h>

#include "aprilparticleExport.h"

namespace aprilparticle
{
	class Particle;
	class System;

	class aprilparticleExport Affector
	{
	public:
		friend class System;

		Affector(chstr name = "");
		virtual ~Affector();

		HL_DEFINE_GETSET(hstr, name, Name);
				
		virtual void update(Particle* particle, float k) { }
		virtual void draw() { }
		virtual hstr getProperty(chstr name, bool* property_exists = NULL);
		virtual bool setProperty(chstr name, chstr value);

	protected:
		hstr name;
		System* system;

		void _setSystem(System* value) { this->system = value; }

	};
}

#endif
