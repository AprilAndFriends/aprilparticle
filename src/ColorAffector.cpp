#include <hltypes/util.h>

#include "ColorAffector.h"
#include "Particle.h"

namespace april
{
	namespace Affectors
	{
		ColorAffector::ColorAffector()
		{
			this->startColor = 0xFFFFFFFF;
			this->endColor = 0xFFFFFFFF;
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
	
		void ColorAffector::update(Particle* particle, double t)
		{
			unsigned int sr, sg, sb, sa, ea, er, eg, eb, ca, cr, cg, cb;
			
			sr = (startColor & 0xFF000000) >> 24;
			sg = (startColor & 0x00FF0000) >> 16;
			sb = (startColor & 0x0000FF00) >> 8;
			sa = (startColor & 0x000000FF);
			
			er = (endColor & 0xFF000000) >> 24;
			eg = (endColor & 0x00FF0000) >> 16;
			eb = (endColor & 0x0000FF00) >> 8;
			ea = (endColor & 0x000000FF);
			
			float life = particle->getLife();
			float totalLife = particle->getTotalLife();
			float ratio = (totalLife > 0.0f ? hmax(life / totalLife, 0.0f) : 1.0f);

			cr = (unsigned int)(ratio * sr + (1.0f - ratio) * er) << 24;
			cg = (unsigned int)(ratio * sg + (1.0f - ratio) * eg) << 16;
			cb = (unsigned int)(ratio * sb + (1.0f - ratio) * eb) << 8;
			ca = (unsigned int)(ratio * sa + (1.0f - ratio) * ea);
			
			particle->setColor(cr | cg | cb | ca);
		}
		
		void ColorAffector::draw()
		{
		}
		
	}

}

