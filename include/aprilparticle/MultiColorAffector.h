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
				
			void update(Particle* emitter, double t);
			void setColors(hmap<float, unsigned int> colors);
			void draw();
			
		protected:
			hmap<float, unsigned int> colors;

		};
		
	}

}

#endif
