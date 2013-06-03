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
/// Represents a linear force affector.

#ifndef APRILPARTICLE_LINEAR_FORCE_H
#define APRILPARTICLE_LINEAR_FORCE_H

#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport LinearForce : public Affector
		{
		public:
			LinearForce(chstr name = "");
			LinearForce(gvec3 direction, chstr name = "");
			~LinearForce();
			static Affector* createInstance(chstr name = "");
			
			HL_DEFINE_GETSET(gvec3, direction, Direction);
			void setDirection(float x, float y, float z) { this->direction.set(x, y, z); }

			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float k, gvec3& movement);

		protected:
			gvec3 direction;
	
		};
	};
}

#endif
