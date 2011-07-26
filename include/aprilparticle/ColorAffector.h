#ifndef COLOR_AFFECTOR_H
#define COLOR_AFFECTOR_H

#include "Affector.h"

namespace april
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport ColorAffector : public Affector
		{
		public:
			ColorAffector();
			ColorAffector(unsigned int endColor);
			ColorAffector(unsigned int startColor, unsigned int endColor);
			~ColorAffector();

			unsigned int getStartColor() { return this->startColor; }
			void setStartColor(unsigned int value) { this->startColor = value; }
			unsigned int getEndColor() { return this->endColor; }
			void setEndColor(unsigned int value) { this->endColor = value; }
				
			void update(Particle* particle, double t);
			void draw();

		protected:
			unsigned int startColor;
			unsigned int endColor;

		};
	
	}

}

#endif
