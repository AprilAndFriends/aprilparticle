#ifndef MULTI_COLOR_AFFECTOR_H
#define MULTI_COLOR_AFFECTOR_H

#include <april/Color.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>

#include "Affector.h"

namespace april
{
	class Particle;

	namespace Affectors
	{
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

		};
		
	}

}

#endif
