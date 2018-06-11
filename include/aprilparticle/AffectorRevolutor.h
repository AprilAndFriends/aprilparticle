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
/// Represents an evolution affector.

#ifndef APRILPARTICLE_AFFECTORS_REVOLUTOR_H
#define APRILPARTICLE_AFFECTORS_REVOLUTOR_H

#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "AffectorSpace.h"
#include "aprilparticleExport.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport Revolutor : public Space
		{
			APRILPARTICLE_CLONEABLE(Revolutor);
		public:
			Revolutor(chstr name = "");
			Revolutor(cgvec3f position, float radius, cgvec3f axis, float evolutionSpeed, bool clockwise, chstr name = "");
			~Revolutor();
			static Affector* createInstance(chstr name = "");

			hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

			HL_DEFINE_GETSET(gvec3f, axis, Axis);
			inline void setAxis(float x, float y, float z) { this->axis.set(x, y, z); }
			HL_DEFINE_GETSET(float, evolutionSpeed, EvolutionSpeed);
			bool isClockwise() const;
			void setClockwise(bool value);
			
			hstr getProperty(chstr name);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float timeDelta, gvec3f& movement);

		protected:
			gvec3f axis;
			float evolutionSpeed;
			float angle;

		private:
			static hmap<hstr, PropertyDescription> _propertyDescriptions;

			gvec3f _position;
			gvec3f _direction;
			float _squaredLength;
		
		};

	}
}
#endif
