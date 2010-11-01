#include "ColorAffector.h"
#include "ParticleEmitter.h"

#include <iostream>
#include <stdio.h>

namespace April
{

	namespace Affectors
	{
		
		ColorAffector::ColorAffector()
		{
			startColor = 0xFFFFFFFF;
			endColor = 0xFFFFFFFF;
		}
		
		ColorAffector::ColorAffector(unsigned int endColor)
		{
			this->startColor = 0xFFFFFFFF;
			this->endColor = endColor;
		}
		
		ColorAffector::ColorAffector(unsigned int startColor, unsigned int endColor)
		{
			this->startColor = startColor;
			this->endColor = endColor;
		}

		ColorAffector::~ColorAffector()
		{
		}
	
		void ColorAffector::update(Particle *particle, double t)
		{
			float ratio;
			unsigned int sr, sg, sb, sa, ea, er, eg, eb, ca, cr, cg, cb;
			
			sa = (startColor & 0xFF000000) >> 24;
			sr = (startColor & 0x00FF0000) >> 16;
			sg = (startColor & 0x0000FF00) >> 8;
			sb = (startColor & 0x000000FF);
			
			ea = (endColor & 0xFF000000) >> 24;
			er = (endColor & 0x00FF0000) >> 16;
			eg = (endColor & 0x0000FF00) >> 8;
			eb = (endColor & 0x000000FF);
			
			if(particle->mTotalLife > 0.0) ratio = particle->mLife / particle->mTotalLife;
			else ratio = 1.0;
			if(ratio < 0.0) ratio = 0.0;
			
			ca = (unsigned int)(ratio * sa + (1.0 - ratio) * ea);
			ca = ca << 24;
			
			cr = (unsigned int)(ratio * sr + (1.0 - ratio) * er);
			cr = cr << 16;
			
			cg = (unsigned int)(ratio * sg + (1.0 - ratio) * eg);
			cg = cg << 8;
			
			cb = (unsigned int)(ratio * sb + (1.0 - ratio) * eb);
				
			particle->mColor = 0x00000000;
			particle->mColor |= ca;
			particle->mColor |= cr;
			particle->mColor |= cg;
			particle->mColor |= cb;
				
		}
		
		void ColorAffector::draw()
		{
		}
		
	}

}

