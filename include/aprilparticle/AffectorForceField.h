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
/// Represents a force field affector.

#ifndef APRILPARTICLE_FORCE_FIELD_H
#define APRILPARTICLE_FORCE_FIELD_H

#include <gtypes/Vector3.h>
#include <hltypes/util.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport ForceField : public Affector
		{
		public:
			ForceField(chstr name = "");
			ForceField(gvec3 position, gvec3 direction, float force, chstr name = "");
			~ForceField();

			HL_DEFINE_GETSET(gvec3, position, Position);
			void setPosition(float x, float y, float z) { this->position.set(x, y, z); }
			HL_DEFINE_GETSET(gvec3, direction, Direction);
			void setDirection(float x, float y, float z) { this->direction.set(x, y, z); }
			HL_DEFINE_GETSET(float, force, Force);
				
			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

			void update(Particle* article, float k);
			void draw();

		protected:
			gvec3 position;
			gvec3 direction;
			float force;

		private:
			float _length;

		};
	};
}

#endif
