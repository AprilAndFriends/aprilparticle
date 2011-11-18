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
/// Represents an attractor affector.

#ifndef APRILPARTICLE_ATTRACTOR_H
#define APRILPARTICLE_ATTRACTOR_H

#include <gtypes/Vector3.h>
#include <hltypes/util.h>

#include "AffectorSpace.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport Attractor : public Space
		{
		public:
			Attractor(chstr name = "");
			Attractor(gvec3 position, float radius, float force, chstr name = "");
			~Attractor();

			HL_DEFINE_GETSET(float, force, Force);
				
			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float k, gvec3& movement);

		protected:
			float force;

		private:
			gvec3 _direction;
			float _factor;
			float _squaredLength;

		};
	};
}

#endif
