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
/// Represents an evolution affector.

#ifndef APRILPARTICLE_EVOLUTOR_H
#define APRILPARTICLE_EVOLUTOR_H

#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>

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
			Revolutor(gvec3 position, float radius, gvec3 axis, float evolutionSpeed, bool clockwise, chstr name = "");
			~Revolutor();
			static Affector* createInstance(chstr name = "");

			HL_DEFINE_GETSET(gvec3, axis, Axis);
			void setAxis(float x, float y, float z) { this->axis.set(x, y, z); }
			HL_DEFINE_GETSET(float, evolutionSpeed, EvolutionSpeed);
			bool isClockwise();
			void setClockwise(bool value);
			
			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float k, gvec3& movement);

		protected:
			gvec3 axis;
			float evolutionSpeed;
			float angle;

		private:
			gvec3 _position;
			gvec3 _direction;
			float _squaredLength;
		
		};
	};
}

#endif
