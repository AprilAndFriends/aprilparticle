#include <hltypes/hmap.h>

#include "MultiColorAffector.h"
#include "Particle.h"

namespace april
{
	namespace Affectors
	{
		MultiColorAffector::MultiColorAffector()
		{
			this->colors[0.0f] = 0xFFFFFFFF;
			this->colors[1.0f] = 0x00FFFFFF;
		}
		
		MultiColorAffector::MultiColorAffector(hmap<float, unsigned int> colors)
		{
			this->colors = colors;
		}

		MultiColorAffector::~MultiColorAffector()
		{
		}
		
		void MultiColorAffector::update(Particle* particle, double t)
		{
			float first;
			float second;
			unsigned int sr, sg, sb, sa, ea, er, eg, eb, ca, cr, cg, cb;
			unsigned int start, end;
			

			float life = particle->getLife();
			float totalLife = particle->getTotalLife();
			float ratio = (totalLife > 0.0f ? hmax(life / totalLife, 0.0f) : 1.0f);
			
			first = 0.0f;
			second = 1.0f;
			
			// first we find the segment in which our color currently lies
			foreach_map (float, unsigned int, it, this->colors)
			{
				second = it->first;
				if (it->first > (1.0f - ratio))
				{
					break;
				}
				first = it->first;
			}
			
			start = this->colors[first];
			end = this->colors[second];
			
			sr = (start & 0xFF000000) >> 24;
			sg = (start & 0x00FF0000) >> 16;
			sb = (start & 0x0000FF00) >> 8;
			sa = (start & 0x000000FF);
			
			er = (end & 0xFF000000) >> 24;
			eg = (end & 0x00FF0000) >> 16;
			eb = (end & 0x0000FF00) >> 8;
			ea = (end & 0x000000FF);
			
			ratio = 1.0f - (second - (1.0f - ratio)) / (second - first);
			
			cr = (unsigned int)(ratio * sr + (1.0f - ratio) * er) << 24;
			cg = (unsigned int)(ratio * sg + (1.0f - ratio) * eg) << 16;
			cb = (unsigned int)(ratio * sb + (1.0f - ratio) * eb) << 8;
			ca = (unsigned int)(ratio * sa + (1.0f - ratio) * ea);
			
			particle->setColor(cr | cg | cb | ca);
		}
		
		void MultiColorAffector::draw()
		{
		}
		
	}

}

