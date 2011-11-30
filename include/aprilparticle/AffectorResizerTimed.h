/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a resizer with timed values.

#ifndef APRILPARTICLE_RESIZER_TIMED_H
#define APRILPARTICLE_RESIZER_TIMED_H

#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>
#include <hltypes/util.h>

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
			ResizerTimed(hmap<float, gvec2> timings, chstr name = "");
			~ResizerTimed();

			HL_DEFINE_GET(harray<float>, times, Times);
			HL_DEFINE_GET(harray<gvec2>, sizes, Sizes);
			void setTimings(hmap<float, gvec2> value);
			void setTimings(chstr value);

			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

			void addTiming(float time, gvec2 size);
			
			void update(Particle* emitter, float k, gvec3& movement);
			
		protected:
			harray<float> times;
			harray<gvec2> sizes;

		private:
			float _ratio;
			int _i;
			int _size;

		};
	};
}

#endif
