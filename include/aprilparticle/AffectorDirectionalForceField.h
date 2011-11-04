/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a directional force field affector.

#ifndef APRILPARTICLE_DIRECTIONAL_FORCE_FIELD_H
#define APRILPARTICLE_DIRECTIONAL_FORCE_FIELD_H

#include <gtypes/Vector3.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport DirectionalForceField : public Affector
		{
		public:
			DirectionalForceField();
			DirectionalForceField(gvec3 position, gvec3 direction, float force);
			~DirectionalForceField();

			gvec3 getPosition() { return this->position; }
			void setPosition(gvec3 value) { this->position = value; }
			float getX() { return this->position.x; }
			void setX(float value) { this->position.x = value; }
			float getY() { return this->position.y; }
			void setY(float value) { this->position.y = value; }
			float getZ() { return this->position.z; }
			void setZ(float value) { this->position.z = value; }
			gvec3 getDirection() { return this->direction; }
			void setDirection(gvec3 value) { this->direction = value; }
			float getDirectionX() { return this->direction.x; }
			void setDirectionX(float value) { this->direction.x = value; }
			float getDirectionY() { return this->direction.y; }
			void setDirectionY(float value) { this->direction.y = value; }
			float getDirectionZ() { return this->direction.z; }
			void setDirectionZ(float value) { this->direction.z = value; }
			float getForce() { return this->force; }
			void setForce(float value) { this->force = value; }
				
			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

			void update(Particle* article, float k);
			void draw();

		protected:
			gvec3 position;
			gvec3 direction;
			float force;

		private:
			float _length;

		};
	};
}

#endif
