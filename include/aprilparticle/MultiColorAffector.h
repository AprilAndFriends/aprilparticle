#ifndef _MULTI_COLOR_AFFECTOR_H_
#define _MULTI_COLOR_AFFECTOR_H_

#include "DirectionalForceField.h"
#include <map>

namespace April
{
	
	namespace Affectors
	{

		class aprilparticleExport MultiColorAffector : public Affector
		{

			public:
			
				std::map<float, unsigned int> mColors;

				MultiColorAffector();
				MultiColorAffector(std::map<float, unsigned int> colors);
				~MultiColorAffector();
				
				void update(Particle *emitter, double t);
				void setColors(std::map<float, unsigned int> colors);
				void draw();

		};
		
	}

}

#endif // _MULTI_COLOR_AFFECTOR_H_
