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
/// Represents a linear force affector.

#ifndef APRILPARTICLE_AFFECTORS_LINEAR_FORCE_H
#define APRILPARTICLE_AFFECTORS_LINEAR_FORCE_H

#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport LinearForce : public Affector
		{
			APRILPARTICLE_CLONEABLE(LinearForce);
		public:
			LinearForce(chstr name = "");
			LinearForce(cgvec3f direction, chstr name = "");
			static Affector* createInstance(chstr name = "");
			
			hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

			HL_DEFINE_GETSET(gvec3f, direction, Direction);
			inline void setDirection(float x, float y, float z) { this->direction.set(x, y, z); }

			hstr getProperty(chstr name);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float timeDelta, gvec3f& movement);

		protected:
			gvec3f direction;
	
		private:
			static hmap<hstr, PropertyDescription> _propertyDescriptions;

		};

	}
}
#endif
