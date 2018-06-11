/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Represents a rotation affector.

#ifndef APRILPARTICLE_AFFECTORS_ROTATOR_H
#define APRILPARTICLE_AFFECTORS_ROTATOR_H

#include <hltypes/hltypesUtil.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport Rotator : public Affector
		{
			APRILPARTICLE_CLONEABLE(Rotator);
		public:
			Rotator(chstr name = "");
			Rotator(float rotationSpeed, chstr name = "");
			~Rotator();
			static Affector* createInstance(chstr name = "");

			hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

			HL_DEFINE_GETSET(float, rotationSpeed, RotationSpeed);
				
			hstr getProperty(chstr name);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float timeDelta, gvec3f& movement);

		protected:
			float rotationSpeed;

		private:
			static hmap<hstr, PropertyDescription> _propertyDescriptions;

		};

	}
}
#endif
