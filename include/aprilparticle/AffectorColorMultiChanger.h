/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a color multi-changer.

#ifndef APRILPARTICLE_COLOR_MULTI_CHANGER_H
#define APRILPARTICLE_COLOR_MULTI_CHANGER_H

#include <april/Color.h>
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
		class aprilparticleExport ColorMultiChanger : public Affector
		{
		public:
			ColorMultiChanger(chstr name = "");
			ColorMultiChanger(hmap<float, april::Color> colorTimings, chstr name = "");
			~ColorMultiChanger();

			HL_DEFINE_GET(harray<float>, times, Times);
			HL_DEFINE_GET(harray<april::Color>, colors, Colors);
			void setColorTimings(hmap<float, april::Color> value);
			void setColorTimings(chstr value);

			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

			void addColorTiming(float time, april::Color color);
			
			void update(Particle* emitter, float k);
			
		protected:
			harray<float> times;
			harray<april::Color> colors;

		private:
			float _ratio;
			int _i;
			int _size;

		};
	};
}

#endif
