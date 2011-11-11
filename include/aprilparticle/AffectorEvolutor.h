/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.3
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
#include <hltypes/util.h>

#include "Affector.h"
#include "aprilparticleExport.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport Evolutor : public Affector
		{
		public:
			Evolutor(chstr name = "");
			Evolutor(gvec3 position, gvec3 axis, float force, float range, bool clockwise, chstr name = "");
			~Evolutor();

			HL_DEFINE_GETSET(gvec3, position, Position);
			void setPosition(float x, float y, float z) { this->position.set(x, y, z); }
			HL_DEFINE_GETSET(gvec3, axis, Axis);
			void setAxis(float x, float y, float z) { this->axis.set(x, y, z); }
			HL_DEFINE_GETSET(float, force, Force);
			HL_DEFINE_GETSET(float, range, Range);
			bool getClockwise();
			void setClockwise(bool value);
			
			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float k);

		protected:
			gvec3 position;
			gvec3 axis;
			float force;
			float range;
			float angle;

		private:
			gvec3 _direction;
			float _squaredLength;
		
		};
	};
}

#endif
