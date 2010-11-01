#include "MultiColorAffector.h"
#include "ParticleEmitter.h"

#include <stdio.h>

namespace April
{
	
	namespace Affectors
	{
	
		MultiColorAffector::MultiColorAffector()
		{
			this->mColors[0.0] = 0xFFFFFFFF;
			this->mColors[1.0] = 0x00FFFFFF;
		}
		
		MultiColorAffector::MultiColorAffector(std::map<float, unsigned int> colors)
		{
			this->mColors = colors;
		}

		MultiColorAffector::~MultiColorAffector()
		{
		}
		
		void MultiColorAffector::draw()
		{
			
		}
		
		void MultiColorAffector::setColors(std::map<float, unsigned int> colors)
		{
			this->mColors = colors;
		}
		
		void MultiColorAffector::update(Particle *particle, double t)
		{
			float ratio, first, second;
			unsigned int sr, sg, sb, sa, ea, er, eg, eb, ca, cr, cg, cb;
			unsigned int start, end;
			
			if(particle->mTotalLife > 0.0) ratio = particle->mLife / particle->mTotalLife;
			else ratio = 1.0;
			if(ratio < 0.0) ratio = 0.0;
			
			first = 0.0;
			second = 1.0;
			
			//first we find the segment in which our color currently lies
			for(std::map<float, unsigned int>::iterator it = mColors.begin(); it != mColors.end(); it++)
			{
				second = it->first;
				if(it->first > (1.0 - ratio))
					break;
				first = it->first;
			}
			
			start 	= mColors[first];
			end 	= mColors[second];
			
			sa = (start & 0xFF000000) >> 24;
			sr = (start & 0x00FF0000) >> 16;
			sg = (start & 0x0000FF00) >> 8;
			sb = (start & 0x000000FF);
			
			ea = (end & 0xFF000000) >> 24;
			er = (end & 0x00FF0000) >> 16;
			eg = (end & 0x0000FF00) >> 8;
			eb = (end & 0x000000FF);
			
			ratio = 1.0 - (second - (1.0 - ratio)) / (second - first);
			
			ca = (unsigned int)(ratio * ea + (1.0 - ratio) * sa);
			ca = ca << 24;
			
			cr = (unsigned int)(ratio * er + (1.0 - ratio) * sr);
			cr = cr << 16;
			
			cg = (unsigned int)(ratio * eg + (1.0 - ratio) * sg);
			cg = cg << 8;
			
			cb = (unsigned int)(ratio * eb + (1.0 - ratio) * sb);
				
			particle->mColor = 0x00000000;
			particle->mColor |= ca;
			particle->mColor |= cr;
			particle->mColor |= cg;
			particle->mColor |= cb;
			
		}
		
	}

}

