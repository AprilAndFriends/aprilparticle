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
/// Represents a swirler affector.

#ifndef APRILPARTICLE_SWIRL_H
#define APRILPARTICLE_SWIRL_H

#include <gtypes/Vector3.h>
#include <hltypes/util.h>

#include "Affector.h"
#include "aprilparticleExport.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport Swirl : public Affector
		{
		public:
			Swirl(chstr name = "");
			Swirl(gvec3 position, gvec3 up, float force, float range, bool clockwise, chstr name = "");
			~Swirl();

			HL_DEFINE_GETSET(gvec3, position, Position);
			HL_DEFINE_GETSET(gvec3, up, Up);
			HL_DEFINE_GETSET(float, force, Force);
			HL_DEFINE_GETSET(float, range, Range);
			float getX() { return this->position.x; }
			void setX(float value) { this->position.x = value; }
			float getY() { return this->position.y; }
			void setY(float value) { this->position.y = value; }
			float getZ() { return this->position.z; }
			void setZ(float value) { this->position.z = value; }
			float getUpX() { return this->up.x; }
			void setUpX(float value) { this->up.x = value; }
			float getUpY() { return this->up.y; }
			void setUpY(float value) { this->up.y = value; }
			float getUpZ() { return this->up.z; }
			void setUpZ(float value) { this->up.z = value; }
			bool getClockwise();
			void setClockwise(bool value);
			
			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float k);

		protected:
			gvec3 position;
			gvec3 up;
			float force;
			float range;
			float angle;

		private:
			gvec3 _direction;
			float _squaredLength;
		
		};
	};
}

#endif
