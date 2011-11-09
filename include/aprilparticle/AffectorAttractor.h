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
/// Represents an attractor affector.

#ifndef APRILPARTICLE_ATTRACTOR_H
#define APRILPARTICLE_ATTRACTOR_H

#include <gtypes/Vector3.h>
#include <hltypes/util.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport Attractor : public Affector
		{
		public:
			Attractor();
			Attractor(gvec3 position, float radius);
			~Attractor();

			HL_DEFINE_GETSET(gvec3, position, Position);
			HL_DEFINE_GETSET(float, radius, Radius);
			float getX() { return this->position.x; }
			void setX(float value) { this->position.x = value; }
			float getY() { return this->position.y; }
			void setY(float value) { this->position.y = value; }
			float getZ() { return this->position.z; }
			void setZ(float value) { this->position.z = value; }
				
			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float k);

		protected:
			gvec3 position;
			float radius;

		private:
			gvec3 _direction;
			float _length;

		};
	};
}

#endif
