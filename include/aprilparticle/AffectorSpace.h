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
/// Represents an Space with a position in 3D space.

#ifndef APRILPARTICLE_AFFECTORS_SPACE_H
#define APRILPARTICLE_AFFECTORS_SPACE_H

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
		class aprilparticleExport Space : public Affector
		{
			APRILPARTICLE_CLONEABLE(Space);
		public:
			Space(chstr name = "");
			Space(cgvec3f position, float space, chstr name = "");
			~Space();
			static Affector* createInstance(chstr name = "");

			hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

			HL_DEFINE_GETSET(gvec3f, position, Position);
			inline void setPosition(float x, float y, float z) { this->position.set(x, y, z); }
			HL_DEFINE_GETSET(float, radius, Radius);

			hstr getProperty(chstr name);
			bool setProperty(chstr name, chstr value);

		protected:
			gvec3f position;
			float radius;

		private:
			static hmap<hstr, PropertyDescription> _propertyDescriptions;

		};

	}
}
#endif
