#ifndef COLOR_AFFECTOR_H
#define COLOR_AFFECTOR_H

#include <april/Color.h>

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
			ColorAffector(april::Color endColor);
			ColorAffector(april::Color startColor, april::Color endColor);
			~ColorAffector();

			april::Color getStartColor() { return this->startColor; }
			void setStartColor(april::Color value) { this->startColor = value; }
			april::Color getEndColor() { return this->endColor; }
			void setEndColor(april::Color value) { this->endColor = value; }
				
			void update(Particle* particle, float k);

		protected:
			april::Color startColor;
			april::Color endColor;

		};
	
	}

}

#endif
