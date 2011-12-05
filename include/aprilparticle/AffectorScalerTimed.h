/// @file
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
/// Represents a scaler with timed values.

#ifndef APRILPARTICLE_SCALER_TIMED_H
#define APRILPARTICLE_SCALER_TIMED_H

#include <april/Color.h>
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
		class aprilparticleExport ScalerTimed : public Affector
		{
		public:
			ScalerTimed(chstr name = "");
			ScalerTimed(hmap<float, float> timings, chstr name = "");
			~ScalerTimed();

			HL_DEFINE_GET(harray<float>, times, Times);
			HL_DEFINE_GET(harray<float>, values, Values);
			void setTimings(hmap<float, float> value);
			void setTimings(chstr value);

			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

			void addTiming(float time, float value);
			
			void update(Particle* emitter, float k, gvec3& movement);
			
		protected:
			harray<float> times;
			harray<float> values;

		private:
			float _ratio;
			int _i;
			int _size;

		};
	};
}

#endif
