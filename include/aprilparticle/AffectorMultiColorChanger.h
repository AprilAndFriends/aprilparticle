/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a multi-color affector.

#ifndef APRILPARTICLE_MULTI_COLOR_CHANGER_H
#define APRILPARTICLE_MULTI_COLOR_CHANGER_H

#include <april/Color.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/util.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport MultiColorChanger : public Affector
		{
		public:
			MultiColorChanger();
			MultiColorChanger(hmap<float, april::Color> timeColors);
			~MultiColorChanger();

			HL_DEFINE_GET(harray<float>, times, Times);
			HL_DEFINE_GET(harray<april::Color>, colors, Colors);
			void setColorTimings(hmap<float, april::Color> timeColors);
			void setColorTimings(chstr timeColors);

			bool setProperty(chstr name, chstr value);

			void addColorTiming(float time, april::Color color);
			
			void update(Particle* emitter, float k);
			void draw();
			
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
