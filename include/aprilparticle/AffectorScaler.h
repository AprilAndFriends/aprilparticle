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
/// Represents a scale affector.

#ifndef APRILPARTICLE_SCALER_H
#define APRILPARTICLE_SCALER_H

#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport Scaler : public Affector
		{
		public:
			Scaler(chstr name = "");
			Scaler(float startScale, float endScale, chstr name = "");
			~Scaler();
			static Affector* createInstance(chstr name = "");

			HL_DEFINE_GETSET(float, startScale, StartScale);
			HL_DEFINE_GETSET(float, endScale, EndScale);
			
			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float k, gvec3& movement);

		protected:
			float startScale;
			float endScale;

		private:
			float _ratio;

		};
	};
}

#endif
