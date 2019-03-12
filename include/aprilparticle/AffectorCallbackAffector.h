/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Represents an affector that uses a callback for the particle affection.

#ifndef APRILPARTICLE_AFFECTORS_CALLBACK_AFFECTOR_H
#define APRILPARTICLE_AFFECTORS_CALLBACK_AFFECTOR_H

#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport CallbackAffector : public Affector
		{
			APRILPARTICLE_CLONEABLE(CallbackAffector);
		public:
			CallbackAffector(chstr name = "");
			CallbackAffector(void (*function)(CallbackAffector*, Particle*, float, gvec3f&), chstr name = "");
			static Affector* createInstance(chstr name = "");

			inline void (*getAffectionCallback())(CallbackAffector*, Particle*, float, gvec3f&) { return this->affectionCallback; }
			inline void setAffectionCallback(void (*value)(CallbackAffector*, Particle*, float, gvec3f&)) { this->affectionCallback = value; }
				
			hstr getProperty(chstr name);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float timeDelta, gvec3f& movement);

		protected:
			hmap<hstr, hstr> properties;
			void (*affectionCallback)(CallbackAffector*, Particle*, float, gvec3f&);

		};

	}
}
#endif
