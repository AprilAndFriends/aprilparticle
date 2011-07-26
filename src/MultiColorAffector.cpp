#include <april/Color.h>
#include <hltypes/hmap.h>

#include "MultiColorAffector.h"
#include "Particle.h"

namespace april
{
	namespace Affectors
	{
		MultiColorAffector::MultiColorAffector()
		{
			this->colors[0.0f] = APRIL_COLOR_WHITE;
			this->colors[1.0f] = april::Color(APRIL_COLOR_WHITE, 0);
		}
		
		MultiColorAffector::MultiColorAffector(hmap<float, april::Color> colors)
		{
			this->colors = colors;
		}

		MultiColorAffector::~MultiColorAffector()
		{
		}
		
		void MultiColorAffector::update(Particle* particle, float k)
		{
			float life = particle->getLife();
			float totalLife = particle->getTotalLife();
			float ratio = (totalLife > 0.0f ? hmax(life / totalLife, 0.0f) : 1.0f);
			float first = 0.0f;
			float second = 1.0f;
			// first we find the segment in which our color currently lies
			foreach_map (float, april::Color, it, this->colors)
			{
				second = it->first;
				if (it->first > (1.0f - ratio))
				{
					break;
				}
				first = it->first;
			}
			ratio = 1.0f - (second - (1.0f - ratio)) / (second - first);
			particle->setColor(this->colors[first] * ratio + this->colors[second] * (1.0f - ratio));
		}
		
		void MultiColorAffector::draw()
		{
		}
		
	}

}

