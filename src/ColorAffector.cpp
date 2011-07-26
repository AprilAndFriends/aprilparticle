#include <april/Color.h>
#include <hltypes/util.h>

#include "ColorAffector.h"
#include "Particle.h"

namespace april
{
	namespace Affectors
	{
		ColorAffector::ColorAffector()
		{
			this->startColor = APRIL_COLOR_WHITE;
			this->endColor = april::Color(APRIL_COLOR_WHITE, 0);
		}
		
		ColorAffector::ColorAffector(april::Color endColor)
		{
			this->startColor = APRIL_COLOR_WHITE;
			this->endColor = endColor;
		}
		
		ColorAffector::ColorAffector(april::Color startColor, april::Color endColor)
		{
			this->startColor = startColor;
			this->endColor = endColor;
		}

		ColorAffector::~ColorAffector()
		{
		}
	
		void ColorAffector::update(Particle* particle, float k)
		{
			float life = particle->getLife();
			float totalLife = particle->getTotalLife();
			float ratio = (totalLife > 0.0f ? hmax(life / totalLife, 0.0f) : 1.0f);
			particle->setColor(this->startColor * ratio + this->endColor * (1.0f - ratio));
		}
		
	}

}

