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
			this->times += 0.0f;
			this->colors += APRIL_COLOR_WHITE;
			this->times += 1.0f;
			this->colors += april::Color(APRIL_COLOR_WHITE, 0);
		}
		
		MultiColorAffector::MultiColorAffector(hmap<float, april::Color> timeColors)
		{
			this->setTimeColors(timeColors);
		}

		MultiColorAffector::~MultiColorAffector()
		{
		}

		void MultiColorAffector::setTimeColors(hmap<float, april::Color> timeColors)
		{
			this->colors.clear();
			this->times = timeColors.keys().sorted();
			foreach (float, it, this->times)
			{
				this->colors += timeColors[*it];
			}
		}
		
		void MultiColorAffector::addTimeColor(float time, april::Color color)
		{
			time = hclamp(time, 0.0f, 1.0f);
			int i;
			for (i = 0; i < this->times.size(); i++)
			{
				if (time > this->times[i])
				{
					break;
				}
			}
			this->times.insert_at(i, time);
			this->colors.insert_at(i, color);
		}

		void MultiColorAffector::update(Particle* particle, float k)
		{
			float ratio = particle->getLifeProgressRatio();
			int i;
			int size = this->times.size() - 1;
			for (i = 0; i < size; i++)
			{
				if (ratio > this->times[i] && ratio <= this->times[i + 1])
				{
					break;
				}
			}
			if (i < size)
			{
				ratio = 1.0f - (this->times[i + 1] - ratio) / (this->times[i + 1] - this->times[i]);
				particle->setColor(this->colors[i] * ratio + this->colors[i + 1] * (1.0f - ratio));
			}
			else
			{
				particle->setColor(this->colors[i]);
			}
		}
		
		void MultiColorAffector::draw()
		{
		}
		
	}

}

