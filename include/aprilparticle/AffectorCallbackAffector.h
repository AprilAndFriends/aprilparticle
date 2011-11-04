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
			CallbackAffector();
			~CallbackAffector();

			void (*getAffectionCallback())(CallbackAffector*, Particle*, float) { return this->affectionCallback; }
			void setAffectionCallback(void (*value)(CallbackAffector*, Particle*, float)) { this->affectionCallback = value; }
				
			void update(Particle* particle, float k);

		protected:
			void (*affectionCallback)(CallbackAffector*, Particle*, float);

		};
	};
}

#endif
