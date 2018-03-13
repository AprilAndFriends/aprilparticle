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
/// Represents a color changer with timed values.

#ifndef APRILPARTICLE_AFFECTORS_COLOR_CHANGER_TIMED_H
#define APRILPARTICLE_AFFECTORS_COLOR_CHANGER_TIMED_H

#include <april/Color.h>
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
		class aprilparticleExport ColorChangerTimed : public Affector
		{
		public:
			ColorChangerTimed(chstr name = "");
			ColorChangerTimed(const hmap<float, april::Color>& timings, chstr name = "");
			~ColorChangerTimed();
			static Affector* createInstance(chstr name = "");

			hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

			HL_DEFINE_GET(harray<float>, times, Times);
			HL_DEFINE_GET(harray<april::Color>, values, Values);
			void setTimings(const hmap<float, april::Color>& value);
			void setTimings(chstr value);

			hstr getProperty(chstr name);
			bool setProperty(chstr name, chstr value);

			void addTiming(float time, const april::Color& value);
			
			void update(Particle* emitter, float timeDelta, gvec3& movement);
			
		protected:
			harray<float> times;
			harray<april::Color> values;

		private:
			static hmap<hstr, PropertyDescription> _propertyDescriptions;

			float _ratio;
			int _i;
			int _size;

		};

	}
}
#endif
