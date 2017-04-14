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
/// Represents a resizer with timed values.

#ifndef APRILPARTICLE_RESIZER_TIMED_H
#define APRILPARTICLE_RESIZER_TIMED_H

#include <gtypes/Vector2.h>
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
		class aprilparticleExport ResizerTimed : public Affector
		{
		public:
			ResizerTimed(chstr name = "");
			ResizerTimed(const hmap<float, gvec2>& timings, chstr name = "");
			~ResizerTimed();
			static Affector* createInstance(chstr name = "");

			HL_DEFINE_GET(harray<float>, times, Times);
			HL_DEFINE_GET(harray<gvec2>, values, Values);
			void setTimings(const hmap<float, gvec2>& value);
			void setTimings(chstr value);

			harray<PropertyDescription> getPropertyDescriptions() const;

			hstr getProperty(chstr name);
			bool setProperty(chstr name, chstr value);

			void addTiming(float time, cgvec2 value);
			
			void update(Particle* emitter, float timeDelta, gvec3& movement);
			
		protected:
			harray<float> times;
			harray<gvec2> values;

		private:
			static harray<PropertyDescription> _propertyDescriptions;

			float _ratio;
			int _i;
			int _size;

		};
	};
}

#endif
