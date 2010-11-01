#ifndef _ROTATOR_H_
#define _ROTATOR_H_

#include "Affector.h"
#include "Particle.h"

namespace April
{

	namespace Affectors
	{

		class aprilparticleExport Rotator : public Affector {

			public:
			
				float mRotationSpeed;
			
				Rotator();
				Rotator(float rotationSpeed);
				~Rotator();
				
				void update(Particle *particle, double t);
				void draw();

		};

	}

}

#endif // _ROTATOR_H_
