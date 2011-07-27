#ifndef PARTICLE_EMITTER_H
#define PARTICLE_EMITTER_H

#include <april/RenderSystem.h>
#include <hltypes/hlist.h>

//#include <string>
#include <deque>
//#include <math.h>

#include "aprilparticleExport.h"
#include "Particle.h"
#include "Affectors.h" // includes all affectors

namespace april
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
	
	class Texture;

	class aprilparticleExport ParticleEmitter : public Particle
	{
	public:
		ParticleEmitter();
		ParticleEmitter(float life, float particlesPerSecond = 60.0f,
						gvec3 position = gvec3(0.0f, 0.0f, 0.0f), gvec3 direction = gvec3(0.0f, 0.0f, 1.0f), unsigned int max = 256);
		~ParticleEmitter();
		
		void setBlendMode(BlendMode value) { this->blendMode = value; }
		void setParticlesEmissionRate(float value) { this->particlesPerSecond = value; }
		void setEmitterType(EmitterType value) { this->emitterType = value; }
		void setRandomStartLife(bool value) { this->randomLife = value; }
		void setRandomStartSize(bool value) { this->randomStartSize = value; }
		void setRandomStartAngle(bool value) { this->randomStartAngle = value; }
		void setTexture(april::Texture* texture) { this->texture = texture; }

		void setMaxParticles(int value);
			
		void setEmitterVolume(float width, float height, float length);
		void setLifeRange(float lifeMin, float lifeMax);
		void setSizeRange(float sizeMin, float sizeMax);
			
		void addAffector(Affectors::Affector* affector);
			
		void update(float k);
		void createParticle();
			
		void draw(gvec3 point, gvec3 up);
		void drawAffectors();
			
	protected:
		april::ColoredTexturedVertex* _triangleBatch;
		std::deque<Particle> particles;
		hlist<Affectors::Affector*> affectors;
		unsigned int maxParticles;
		float width;
		float height;
		float length;
		BlendMode blendMode;
		EmitterType emitterType;
		bool randomStartAngle;
		bool randomStartSize;
		bool randomLife;
		float particlesPerSecond;
		float counter;
		float minSize;
		float maxSize;
		float lifeMin;
		float lifeMax;
		april::Texture* texture;

		void _setupTriangleBatch();
		
	};

}

#endif // PARTICLE_EMITTER_H
