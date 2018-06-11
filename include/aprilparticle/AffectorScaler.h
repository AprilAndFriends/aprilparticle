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
/// Represents a scale affector.

#ifndef APRILPARTICLE_AFFECTORS_SCALER_H
#define APRILPARTICLE_AFFECTORS_SCALER_H

#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport Scaler : public Affector
		{
			APRILPARTICLE_CLONEABLE(Scaler);
		public:
			Scaler(chstr name = "");
			Scaler(float startScale, float endScale, chstr name = "");
			~Scaler();
			static Affector* createInstance(chstr name = "");

			hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

			HL_DEFINE_GETSET(float, startScale, StartScale);
			HL_DEFINE_GETSET(float, endScale, EndScale);
			
			hstr getProperty(chstr name);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float timeDelta, gvec3f& movement);

		protected:
			float startScale;
			float endScale;

		private:
			static hmap<hstr, PropertyDescription> _propertyDescriptions;

			float _ratio;

		};

	}
}
#endif
