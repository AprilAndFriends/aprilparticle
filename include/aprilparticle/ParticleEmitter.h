/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a particle emitter.

#ifndef APRILPARTICLE_PARTICLE_EMITTER_H
#define APRILPARTICLE_PARTICLE_EMITTER_H

#include <april/RenderSystem.h>
#include <gtypes/Matrix3.h>
#include <gtypes/Matrix4.h>
#include <gtypes/Vector3.h>
#include <hltypes/hdeque.h>
#include <hltypes/hlist.h>

#include "Affectors.h" // includes all affectors
#include "aprilparticleExport.h"
#include "Particle.h"

namespace april
{
	class Texture;
}

namespace aprilparticle
{
	class Affector;

	class aprilparticleExport ParticleEmitter : public Particle
	{
	public:
		enum Type
		{
			Point,
			Box,
			Sphere,
			HollowSphere,
			Ring,
			Cylinder,
			HollowCylinder
		};
	
		ParticleEmitter();
		ParticleEmitter(float life, float emissionRate = 60.0f, gvec3 position = gvec3(0.0f, 0.0f, 0.0f),
			gvec3 direction = gvec3(0.0f, 0.0f, 1.0f), unsigned int max = 256);
		~ParticleEmitter();

		void setParticleScale(float value);



		void setParticleScaleRange(float min, float max);
		void setParticleAngleRange(float min, float max);

		
		void setBlendMode(april::BlendMode value) { this->blendMode = value; }
		void setParticlesEmissionRate(float value) { this->emissionRate = value; }
		void setEmitterType(Type value) { this->emitterType = value; }
		void setTexture(april::Texture* texture) { this->texture = texture; }

		void setMaxParticles(int value);
			
		void setEmitterVolume(float width, float height, float length);
		void setLifeRange(float minLife, float maxLife);
		void setSizeRange(float minParticleScale, float maxParticleScale);
			
		void addAffector(Affector* affector);
			
		void update(float k);
		void createParticle();
			
		void draw(gvec3 point, gvec3 up);
		void drawAffectors(); // usually only used for debug purposes
			
	protected:
		april::ColoredTexturedVertex* _triangleBatch;
		hdeque<Particle> particles;
		hlist<Affector*> affectors;
		int maxParticles;
		float width;
		float height;
		float length;
		april::BlendMode blendMode;
		Type emitterType;
		float emissionRate;
		float timer;
		float minParticleScale;
		float maxParticleScale;
		float minLife;
		float maxLife;
		april::Texture* texture;

		void _setupTriangleBatch();

	private:
		gvec3 _pos;
		float _rho;
		float _phi;
		float _theta;
		float _S;
		float _cs;
		int _quota;
		gmat4 _billboard;
		gmat3 _rot;
		int _i;
		unsigned int _color;
		
	};
}

#endif // PARTICLE_EMITTER_H
