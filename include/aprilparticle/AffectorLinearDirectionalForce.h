/// @file
/// @author  Domagoj Cerjan
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
/// Represents a linear directional force affector.

#ifndef APRILPARTICLE_LINEAR_DIRECTIONAL_FORCE_H
#define APRILPARTICLE_LINEAR_DIRECTIONAL_FORCE_H

#include <gtypes/Vector3.h>
#include <hltypes/util.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport LinearDirectionalForce : public Affector
		{
		public:
			LinearDirectionalForce(gvec3 direction = gvec3(0.0f, -1.0f, 0.0f), float force = 0.001f);
			~LinearDirectionalForce();
			
			HL_DEFINE_GETSET(gvec3, direction, Direction);
			HL_DEFINE_GETSET(float, force, Force);
			float getDirectionX() { return this->direction.x; }
			void setDirectionX(float value) { this->direction.x = value; }
			float getDirectionY() { return this->direction.y; }
			void setDirectionY(float value) { this->direction.y = value; }
			float getDirectionZ() { return this->direction.z; }
			void setDirectionZ(float value) { this->direction.z = value; }

			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float k);

		protected:
			gvec3 direction;
			float force;
	
		};
	};
}

#endif
