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

	class aprilparticleExport DirectionalForceField : public Affector
	{
	public:
		DirectionalForceField();
		DirectionalForceField(gvec3 position, gvec3 direction, float force);
		~DirectionalForceField();

		gvec3 getPosition() { return this->position; }
		void setPosition(gvec3 value) { this->position = value; }
		gvec3 getDirection() { return this->direction; }
		void setDirection(gvec3 value) { this->direction = value; }
		float getForce() { return this->force; }
		void setForce(float value) { this->force = value; }
				
		void update(Particle* article, float k);
		void draw();

	protected:
		gvec3 position;
		gvec3 direction;
		float force;

	private:
		float _length;

	};

}

#endif
