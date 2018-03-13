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
			Resizer(cgvec2 startSize, cgvec2 endSize, chstr name = "");
			~Resizer();
			static Affector* createInstance(chstr name = "");

			hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

			HL_DEFINE_GETSET(gvec2, startSize, StartSize);
			inline void setStartSize(float x, float y) { this->startSize.set(x, y); }
			HL_DEFINE_GETSET(gvec2, endSize, EndSize);
			inline void setEndSize(float x, float y) { this->endSize.set(x, y); }
				
			hstr getProperty(chstr name);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float timeDelta, gvec3& movement);

		protected:
			gvec2 startSize;
			gvec2 endSize;

		private:
			static hmap<hstr, PropertyDescription> _propertyDescriptions;

			float _ratio;

		};

	}
}
#endif
