#ifndef _COLOR_AFFECTOR_H_
#define _COLOR_AFFECTOR_H_

#include "DirectionalForceField.h"

namespace April
{
	
	namespace Affectors
	{

		class aprilparticleExport ColorAffector : public Affector
		{

			public:
			
				unsigned int startColor, endColor;

				ColorAffector();
				ColorAffector(unsigned int endColor);
				ColorAffector(unsigned int startColor, unsigned int endColor);
				~ColorAffector();
				
				void update(Particle *particle, double t);
				void draw();

		};
	
	}

}

#endif // _COLOR_AFFECTOR_H_
