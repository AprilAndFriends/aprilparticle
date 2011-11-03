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
/// Represents a generic affector class.

#ifndef APRILPARTICLE_AFFECTOR_H
#define APRILPARTICLE_AFFECTOR_H

#include "aprilparticleExport.h"

namespace aprilparticle
{
	class Particle;

	class aprilparticleExport Affector
	{
	public:
		Affector() { }
		virtual ~Affector() { }
				
		virtual void update(Particle* particle, float k) { }
		virtual void draw() { }

	};

}

#endif
