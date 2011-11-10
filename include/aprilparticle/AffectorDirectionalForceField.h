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
/// Represents a directional force field affector.

#ifndef APRILPARTICLE_DIRECTIONAL_FORCE_FIELD_H
#define APRILPARTICLE_DIRECTIONAL_FORCE_FIELD_H

#include <gtypes/Vector3.h>
#include <hltypes/util.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport DirectionalForceField : public Affector
		{
		public:
			DirectionalForceField(chstr name = "");
			DirectionalForceField(gvec3 position, gvec3 direction, float force, chstr name = "");
			~DirectionalForceField();

			HL_DEFINE_GETSET(gvec3, position, Position);
			HL_DEFINE_GETSET(gvec3, direction, Direction);
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
