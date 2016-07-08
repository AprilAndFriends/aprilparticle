/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Represents a scale affector.

#ifndef APRILPARTICLE_SCALER_H
#define APRILPARTICLE_SCALER_H

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
		public:
			Scaler(chstr name = "");
			Scaler(float startScale, float endScale, chstr name = "");
			~Scaler();
			static Affector* createInstance(chstr name = "");

			HL_DEFINE_GETSET(float, startScale, StartScale);
			HL_DEFINE_GETSET(float, endScale, EndScale);
			
			harray<PropertyDescription> getPropertyDescriptions() const;

			hstr getProperty(chstr name);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float timeDelta, gvec3& movement);

		protected:
			float startScale;
			float endScale;

		private:
			static harray<PropertyDescription> _propertyDescriptions;

			float _ratio;

		};
	};
}

#endif
