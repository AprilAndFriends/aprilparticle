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
/// Represents a force field affector.

#ifndef APRILPARTICLE_FORCE_FIELD_H
#define APRILPARTICLE_FORCE_FIELD_H

#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>

#include "AffectorSpace.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport ForceField : public Space
		{
		public:
			ForceField(chstr name = "");
			ForceField(gvec3 position, float radius, gvec3 direction, chstr name = "");
			~ForceField();
			static Affector* createInstance(chstr name = "");

			HL_DEFINE_GETSET(gvec3, direction, Direction);
			void setDirection(float x, float y, float z) { this->direction.set(x, y, z); }
				
			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

			void update(Particle* article, float k, gvec3& movement);
			void draw();

		protected:
			gvec3 direction;

		private:
			float _factor;
			float _squaredLength;
			
		};
	};
}

#endif
