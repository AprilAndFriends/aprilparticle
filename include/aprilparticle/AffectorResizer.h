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
/// Represents a resizing affector.

#ifndef APRILPARTICLE_AFFECTORS_RESIZER_H
#define APRILPARTICLE_AFFECTORS_RESIZER_H

#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport Resizer : public Affector
		{
		public:
			Resizer(chstr name = "");
			Resizer(cgvec2f startSize, cgvec2f endSize, chstr name = "");
			~Resizer();
			static Affector* createInstance(chstr name = "");

			hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

			HL_DEFINE_GETSET(gvec2f, startSize, StartSize);
			inline void setStartSize(float x, float y) { this->startSize.set(x, y); }
			HL_DEFINE_GETSET(gvec2f, endSize, EndSize);
			inline void setEndSize(float x, float y) { this->endSize.set(x, y); }
				
			hstr getProperty(chstr name);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float timeDelta, gvec3f& movement);

		protected:
			gvec2f startSize;
			gvec2f endSize;

		private:
			static hmap<hstr, PropertyDescription> _propertyDescriptions;

			float _ratio;

		};

	}
}
#endif
