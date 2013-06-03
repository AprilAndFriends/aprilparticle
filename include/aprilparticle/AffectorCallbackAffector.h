/// @file
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
/// Represents an affector that uses a callback for the particle affection.

#ifndef APRILPARTICLE_CALLBACK_AFFECTOR_H
#define APRILPARTICLE_CALLBACK_AFFECTOR_H

#include <gtypes/Vector3.h>
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
		public:
			CallbackAffector(chstr name = "");
			CallbackAffector(void (*function)(CallbackAffector*, Particle*, float, gvec3&), chstr name = "");
			~CallbackAffector();
			static Affector* createInstance(chstr name = "");

			void (*getAffectionCallback())(CallbackAffector*, Particle*, float, gvec3&) { return this->affectionCallback; }
			void setAffectionCallback(void (*value)(CallbackAffector*, Particle*, float, gvec3&)) { this->affectionCallback = value; }
				
			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float k, gvec3& movement);

		protected:
			hmap<hstr, hstr> properties;
			void (*affectionCallback)(CallbackAffector*, Particle*, float, gvec3&);

		};
	};
}

#endif
