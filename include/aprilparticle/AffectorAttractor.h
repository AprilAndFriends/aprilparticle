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
/// Represents an attractor affector.

#ifndef APRILPARTICLE_ATTRACTOR_H
#define APRILPARTICLE_ATTRACTOR_H

#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>

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
			Attractor(gvec3 position, float radius, float force, float exponent, chstr name = "");
			~Attractor();
			static Affector* createInstance(chstr name = "");

			HL_DEFINE_GETSET(float, force, Force);
			HL_DEFINE_GETSET(float, exponent, Exponent);
				
			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float k, gvec3& movement);

		protected:
			float force;
			float exponent;

		private:
			gvec3 _direction;
			float _factor;
			float _squaredLength;

		};
	};
}

#endif
