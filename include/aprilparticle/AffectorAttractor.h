/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Represents an attractor affector.

#ifndef APRILPARTICLE_AFFECTORS_ATTRACTOR_H
#define APRILPARTICLE_AFFECTORS_ATTRACTOR_H

#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "AffectorSpace.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport Attractor : public Space
		{
			APRILPARTICLE_CLONEABLE(Attractor);
		public:
			Attractor(chstr name = "");
			Attractor(cgvec3f position, float radius, float force, float exponent, chstr name = "");
			~Attractor();
			static Affector* createInstance(chstr name = "");

			hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

			HL_DEFINE_GETSET(float, force, Force);
			HL_DEFINE_GETSET(float, exponent, Exponent);
				
			hstr getProperty(chstr name);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float timeDelta, gvec3f& movement);

		protected:
			float force;
			float exponent;

		private:
			static hmap<hstr, PropertyDescription> _propertyDescriptions;

			gvec3f _direction;
			float _factor;
			float _squaredLength;

		};

	}
}
#endif
