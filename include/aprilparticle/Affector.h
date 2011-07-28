#ifndef AFFECTOR_H
#define AFFECTOR_H

#include "aprilparticleExport.h"

namespace aprilparticle
{
	class Particle;

	class aprilparticleExport Affector
	{
	public:
		Affector() { }
		virtual ~Affector() { }
				
		virtual void update(Particle* particle, float k) { }
		virtual void draw() { }

	};

}

#endif
