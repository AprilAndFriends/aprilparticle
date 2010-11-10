#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include "aprilparticleExport.h"
#include "april/RenderSystem.h"
#include "Particle.h"
#include "Affectors.h" // includes all affectors
#include <string>
#include <deque>
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
		PDT_AlphaBlend = 1,
		PDT_Normal = 2
	};

	class aprilparticleExport ParticleEmitter : public Particle
	{
			
			April::ColoredTexturedVertex *_mTriangleBatch;
			unsigned int 			mMaxParticles;
			
		public:
		
			std::deque<Particle> 					mParticles;
			std::list<Affectors::Affector*> 		mAffecttors;
			
			
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
			
			April::Texture* 		mTex;
	
			ParticleEmitter();
			ParticleEmitter(float life, float particlesPerSecond = 60,
							gvec3 position = gvec3(0,0,0), gvec3 direction = gvec3(0,0,1), unsigned int max = 256);
			~ParticleEmitter();
			
			void setEmitterType(EmitterType type);
			void setEmiterVolume(float width, float height, float length);
			void setParticleDrawType(ParticlesDrawType type) { mDrawType = type; }
			void setTexture(std::string texture);
			void setMaxParticles(int maxParticles);
			void setParticlesEmissionRate(int particlesEmissionRate) { mParticlesPerSecond = particlesEmissionRate; }
			void setLifeRange(float lifeMin, float lifeMax) { mLifeMin = lifeMin; mLifeMax = lifeMax; }
			void setLife(float life) { mLife = life; }
			void setSizeRange(float sizeMin, float sizeMax) { mMinSize = sizeMin; mMaxSize = sizeMax; }
			
			void setRandomStartLife(bool startLife) { mRandomLife = startLife; }
			void setRandomStartSize(bool startSize) { mRandomStartSize = startSize; }
			void setRandomStartAngle(bool startAngle) { mRandomStartAngle = startAngle; }
			
			
			void addAffector(Affectors::Affector *affector);
			
			void update(float t);
			void createParticle();
			
			void draw(gvec3 point, gvec3 up);
			void drawAffectors();
			
	};

}

#endif // PARTICLE_EMITTER_H
