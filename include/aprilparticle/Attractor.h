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
/// Represents an attractor affector.

#ifndef APRILPARTICLE_ATTRACTOR_H
#define APRILPARTICLE_ATTRACTOR_H

#include <gtypes/Vector3.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	class aprilparticleExport Attractor : public Affector
	{
	public:
		Attractor();
		Attractor(gvec3 position, float radius);
		~Attractor();

		gvec3 getPosition() { return this->position; }
		void setPosition(gvec3 value) { this->position = value; }
		float getRadius() { return this->radius; }
		void setRadius(float value) { this->radius = value; }
				
		void update(Particle* particle, float k);

	protected:
		gvec3 position;
		float radius;

	private:
		gvec3 _direction;
		float _length;

	};

}

#endif
