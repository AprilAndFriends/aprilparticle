/// @file
/// @author  Domagoj Cerjan
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
/// Represents a generic affector class.

#ifndef APRILPARTICLE_AFFECTOR_H
#define APRILPARTICLE_AFFECTOR_H

#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "aprilparticleExport.h"

namespace aprilparticle
{
	class Particle;
	class Space;
	class System;

	class aprilparticleExport Affector
	{
	public:
		friend class System;
		friend class Space;

		Affector(chstr name = "");
		virtual ~Affector();

		HL_DEFINE_GETSET(hstr, name, Name);
		HL_DEFINE_GETSET(float, minRandomness, MinRandomness);
		HL_DEFINE_GETSET(float, maxRandomness, MaxRandomness);
		HL_DEFINE_GETSET(gvec3, chaoticity, Chaoticity);
		void setRandomness(float value);
		void setRandomness(chstr value);

		void setRandomnessRange(float min, float max);
				
		virtual void update(Particle* particle, float k, gvec3& movement) { }
		virtual void draw() { }

		virtual hstr getProperty(chstr name, bool* property_exists = NULL);
		virtual bool setProperty(chstr name, chstr value);

	protected:
		hstr name;
		float minRandomness;
		float maxRandomness;
		gvec3 chaoticity;
		Space* space;

		void _setSpace(Space* value) { this->space = value; }

	};
}

#endif
