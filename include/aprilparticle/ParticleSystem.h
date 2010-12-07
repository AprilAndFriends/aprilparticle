#ifndef PARTICLE_SYSTEM_H
#define PARTICLE_SYSTEM_H

#include <map>
#include <list>
#include "aprilparticleExport.h"
#include "April/RenderSystem.h"
#include "CompositeParticleObject.h"
#include "xmlHelper.h"


namespace April
{
	class aprilparticleExport ParticleSystem
	{
		std::map<April::Texture*, int> _mTextures;
		std::list<April::CompositeParticleObject*> _mComposites;
			
		int _mNumerator;
		
		public:
		
		ParticleSystem();
		~ParticleSystem();
		
		April::CompositeParticleObject* loadParticleObject(hstr fileName, hstr name=hstr(""));
		
		void update(double t);
		void draw();
		
		April::CompositeParticleObject* getParticleObject(hstr name);
		
	};
}

#endif