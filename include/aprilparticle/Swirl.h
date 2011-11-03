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
/// Represents a swirler affector.

#ifndef APRILPARTICLE_SWIRL_H
#define APRILPARTICLE_SWIRL_H

#include <gtypes/Vector3.h>

#include "Affector.h"
#include "aprilparticleExport.h"

namespace aprilparticle
{
	class Particle;

	class aprilparticleExport Swirl : public Affector
	{
	public:
		Swirl();
		Swirl(gvec3 position, gvec3 up, float range, float force);
		~Swirl();

		float getRange() { return this->range; }
		void setRange(float value) { this->range = value; }
		float getForce() { return this->force; }
		void setForce(float value) { this->force = value; }
		gvec3 getPosition() { return this->position; }
		void setPosition(gvec3 value) { this->position = value; }
		gvec3 getUp() { return this->up; }
		void setUp(gvec3 value) { this->up = value; }
		bool getClockwise();
		void setClockwise(bool value);
			
		void update(Particle* particle, float k);

	protected:
		float range;
		float force;
		gvec3 position;
		gvec3 up;
		float angle;

	private:
		gvec3 _direction;
		float _squaredLength;
		
	};

}

#endif
