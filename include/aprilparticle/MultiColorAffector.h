#ifndef MULTI_COLOR_AFFECTOR_H
#define MULTI_COLOR_AFFECTOR_H

#include <april/Color.h>
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
			MultiColorAffector(hmap<float, april::Color> colors);
			~MultiColorAffector();

			hmap<float, april::Color> getColors() { return this->colors; }
			void setColors(hmap<float, april::Color> value) { this->colors = value; }
				
			void update(Particle* emitter, float k);
			void draw();
			
		protected:
			hmap<float, april::Color> colors;

		};
		
	}

}

#endif
