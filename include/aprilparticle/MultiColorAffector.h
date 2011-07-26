#ifndef MULTI_COLOR_AFFECTOR_H
#define MULTI_COLOR_AFFECTOR_H

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
			MultiColorAffector(hmap<float, unsigned int> colors);
			~MultiColorAffector();

			hmap<float, unsigned int> getColors() { return this->colors; }
			void setColors(hmap<float, unsigned int> value) { this->colors = value; }
				
			void update(Particle* emitter, double t);
			void draw();
			
		protected:
			hmap<float, unsigned int> colors;

		};
		
	}

}

#endif
