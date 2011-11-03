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

#ifndef APRILPARTICLE_MULTI_COLOR_AFFECTOR_H
#define APRILPARTICLE_MULTI_COLOR_AFFECTOR_H

#include <april/Color.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	class aprilparticleExport MultiColorAffector : public Affector
	{
	public:
		MultiColorAffector();
		MultiColorAffector(hmap<float, april::Color> timeColors);
		~MultiColorAffector();

		harray<float> getTimes() { return this->times; }
		void setTimes(harray<float> value) { this->times = value; }
		harray<april::Color> getColors() { return this->colors; }
		void setColors(harray<april::Color> value) { this->colors = value; }

		void setTimeColors(hmap<float, april::Color> timeColors);

		void addTimeColor(float time, april::Color color);
				
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

}

#endif
