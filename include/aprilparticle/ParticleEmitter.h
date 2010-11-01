#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "aprilparticleExport.h"
#include "april/RenderSystem.h"
#include "Particle.h"
#include "Affectors.h" // includes all affectors
#include <string>
#include <list>
#include <math.h>

namespace April
{
	enum EmitterType
	{
		ET_Point = 0,
		ET_Box = 1,
		ET_Sphere = 2,
		ET_HollowSphere = 3,
		ET_Ring = 4,
		ET_Cylinder = 5,
		ET_HollowCylinder = 6
	};
	
	enum ParticlesDrawType
	{
		PDT_Additive = 0,
		PDT_Subtractive = 1,
		PDT_Normal = 2
	};

	class aprilparticleExport ParticleEmitter : public Particle
	{
		public:
		
			std::list<Particle> 	mParticles;
			std::list<Affectors::Affector*> 	mAffecttors;
			
			float 					mWidth, mHeigth, mLenght;
			ParticlesDrawType 		mDrawType;
			EmitterType 			mEmitterType;
			
			bool 					mRandomStartAngle;
			bool 					mRandomStartSize;
			bool 					mRandomLife;
			
			float 					mParticlesPerSecond;
			float 					mCounter;
			float 					mMinSize, mMaxSize, mSize;
			
			float 					mLifeMin, mLifeMax;
			
			unsigned int 			mMaxParticles;
			
			April::Texture* 		mTex;
		
			ParticleEmitter();
			ParticleEmitter(float life, float particlesPerSecond = 60,
							gvec3 position = gvec3(0,0,0), gvec3 direction = gvec3(0,0,1), unsigned int max = 256);
			~ParticleEmitter();
			
			void setEmitterType(EmitterType type);
			void setEmiterVolume(float width, float height, float length);
			
			void setParticleDrawType(ParticlesDrawType type);
			
			void setTexture(std::string texture);
			void addAffector(Affectors::Affector *affector);
			
			void update(float t);
			void createParticle();
			
			void draw();
			void drawAffectors();
			
	};

}

#endif // PARTICLE_EMITTER_H
