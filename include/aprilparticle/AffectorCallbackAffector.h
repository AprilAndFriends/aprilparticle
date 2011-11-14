/// @file
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
/// Represents an affector that uses a callback for the particle affection.

#ifndef APRILPARTICLE_CALLBACK_AFFECTOR_H
#define APRILPARTICLE_CALLBACK_AFFECTOR_H

#include <gtypes/Vector3.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport CallbackAffector : public Affector
		{
		public:
			CallbackAffector(chstr name = "");
			CallbackAffector(void (*function)(CallbackAffector*, Particle*, float, gvec3&), chstr name = "");
			~CallbackAffector();

			void (*getAffectionCallback())(CallbackAffector*, Particle*, float, gvec3&) { return this->affectionCallback; }
			void setAffectionCallback(void (*value)(CallbackAffector*, Particle*, float, gvec3&)) { this->affectionCallback = value; }
				
			void update(Particle* particle, float k, gvec3& movement);

		protected:
			void (*affectionCallback)(CallbackAffector*, Particle*, float, gvec3&);

		};
	};
}

#endif
