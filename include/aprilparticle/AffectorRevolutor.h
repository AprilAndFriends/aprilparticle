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
/// Represents an evolution affector.

#ifndef APRILPARTICLE_EVOLUTOR_H
#define APRILPARTICLE_EVOLUTOR_H

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
		public:
			Revolutor(chstr name = "");
			Revolutor(cgvec3 position, float radius, cgvec3 axis, float evolutionSpeed, bool clockwise, chstr name = "");
			~Revolutor();
			static Affector* createInstance(chstr name = "");

			HL_DEFINE_GETSET(gvec3, axis, Axis);
			inline void setAxis(float x, float y, float z) { this->axis.set(x, y, z); }
			HL_DEFINE_GETSET(float, evolutionSpeed, EvolutionSpeed);
			bool isClockwise() const;
			void setClockwise(bool value);
			
			harray<PropertyDescription> getPropertyDescriptions() const;

			hstr getProperty(chstr name);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float timeDelta, gvec3& movement);

		protected:
			gvec3 axis;
			float evolutionSpeed;
			float angle;

		private:
			static harray<PropertyDescription> _propertyDescriptions;

			gvec3 _position;
			gvec3 _direction;
			float _squaredLength;
		
		};
	};
}

#endif
