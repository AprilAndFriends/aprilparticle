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
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/hdeque.h>
#include <hltypes/hlist.h>

#include "Affectors.h" // includes all affectors
#include "aprilparticleExport.h"
#include "particle.h"
#include "Space3DObject.h"

namespace april
{
	class Texture;
}

namespace aprilparticle
{
	class Affector;

	class aprilparticleExport ParticleEmitter : public Space3DObject
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
	
		ParticleEmitter(gvec3 position = gvec3(0.0f, 0.0f, 0.0f), gvec3 direction = gvec3(0.0f, 0.0f, 1.0f),
			float particleLife = 1.0f, float emissionRate = 60.0f, unsigned int max = 256);
		~ParticleEmitter();

		gvec3 getSize() { return this->size; }
		void setSize(gvec3 value) { this->size = value; }
		void setSize(float x, float y, float z) { this->size.set(x, y, z); }
		void setParticleLife(float value);
		void setParticleSize(gvec2 value);
		void setParticleScale(float value);
		void setParticleSpeed(float value);
		void setParticleAngle(float value);
		void setEmissionRate(float value) { this->emissionRate = value; }
		void setParticleLimit(int value);
		Type getType() { return this->type; }
		void setType(Type value) { this->type = value; }
		april::BlendMode getBlendMode() { return this->blendMode; }
		void setBlendMode(april::BlendMode value) { this->blendMode = value; }
		// TODO
		april::Texture* getTexture() { return this->texture; }
		void setTexture(april::Texture* texture) { this->texture = texture; }
		harray<Affector*> getAffectors() { return this->affectors; }

		void setParticleLifeRange(float min, float max);
		void setParticleSizeRange(gvec2 min, gvec2 max);
		void setParticleScaleRange(float min, float max);
		void setParticleSpeedRange(float min, float max);
		void setParticleAngleRange(float min, float max);

		void addAffector(Affector* affector);
		void removeAffector(Affector* affector);
		void registerAffector(Affector* affector);
		void unregisterAffector(Affector* affector);
		void createParticle();

		void update(float k);
		
		void draw(gvec3 point, gvec3 up);
		void drawAffectors(); // usually only used for debug purposes
		
	protected:
		float timer;
		gvec3 size;
		float minParticleLife;
		float maxParticleLife;
		gvec2 minParticleSize;
		gvec2 maxParticleSize;
		float minParticleScale;
		float maxParticleScale;
		float minParticleSpeed;
		float maxParticleSpeed;
		float minParticleAngle;
		float maxParticleAngle;
		float emissionRate;
		int particleLimit;
		Type type;
		april::BlendMode blendMode;
		april::Texture* texture;
		hdeque<Particle> particles;
		harray<Affector*> affectors;
		harray<Affector*> registeredAffectors;

	private:
		april::ColoredTexturedVertex* _triangleBatch;
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
		
		void _setupTriangleBatch();

	};
}

#endif // PARTICLE_EMITTER_H
