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
/// Represents a rotation affector.

#ifndef APRILPARTICLE_ROTATOR_H
#define APRILPARTICLE_ROTATOR_H

#include <hltypes/hltypesUtil.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport Rotator : public Affector
		{
		public:
			Rotator(chstr name = "");
			Rotator(float rotationSpeed, chstr name = "");
			~Rotator();
			static Affector* createInstance(chstr name = "");

			HL_DEFINE_GETSET(float, rotationSpeed, RotationSpeed);
				
			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float k, gvec3& movement);

		protected:
			float rotationSpeed;

		};
	};
}

#endif
