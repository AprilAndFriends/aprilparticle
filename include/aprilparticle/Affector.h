#ifndef _AFFECTOR_H_
#define _AFFECTOR_H_

#include "aprilparticleExport.h"
#include "Particle.h"
#include "gtypes/Vector3.h"
#include <vector>
#include <iostream>

namespace April
{
	
	namespace Affectors
	{

		class aprilparticleExport Affector
		{

			public:
				Affector();
				~Affector();
				
				virtual void update(Particle *particle, double t);
				virtual void draw();

		};
		
	}

}

#endif // _AFFECTOR_H_
