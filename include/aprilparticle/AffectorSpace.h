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
/// Represents an Space with a position in 3D space.

#ifndef APRILPARTICLE_Space_H
#define APRILPARTICLE_Space_H

#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport Space : public Affector
		{
		public:
			Space(chstr name = "");
			Space(gvec3 position, float space, chstr name = "");
			~Space();
			static Affector* createInstance(chstr name = "");

			HL_DEFINE_GETSET(gvec3, position, Position);
			void setPosition(float x, float y, float z) { this->position.set(x, y, z); }
			HL_DEFINE_GETSET(float, radius, Radius);
				
			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

		protected:
			gvec3 position;
			float radius;

		};
	};
}

#endif
