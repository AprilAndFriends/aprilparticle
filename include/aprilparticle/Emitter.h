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

#ifndef APRILPARTICLE_EMITTER_H
#define APRILPARTICLE_EMITTER_H

#include <april/RenderSystem.h>
#include <gtypes/Matrix3.h>
#include <gtypes/Matrix4.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/hdeque.h>
#include <hltypes/hlist.h>
#include <hltypes/util.h>

#include "aprilparticleExport.h"
#include "Space3DObject.h"

namespace april
{
	class Texture;
}

namespace aprilparticle
{
	class Affector;
	class Particle;

	class aprilparticleExport Emitter : public Space3DObject
	{
	public:
		enum Type
		{
			Point,
			Box,
			HollowBox,
			Sphere,
			HollowSphere,
			Cylinder,
			HollowCylinder,
			Circle,
			Ring
		};
	
		Emitter(gvec3 position = gvec3(0.0f, 0.0f, 0.0f), gvec3 direction = gvec3(0.0f, 0.0f, 1.0f),
			float particleLife = 1.0f, float emissionRate = 60.0f, unsigned int max = 256);
		~Emitter();

		HL_DEFINE_GETSET(hstr, name, Name);
		HL_DEFINE_GETSET(gvec3, dimensions, Dimensions);
		void setDimensions(float x, float y, float z) { this->dimensions.set(x, y, z); }
		HL_DEFINE_GETSET(float, minLife, MinLife);
		HL_DEFINE_GETSET(float, maxLife, MaxLife);
		HL_DEFINE_GETSET(gvec2, minSize, MinSize);
		HL_DEFINE_GETSET(gvec2, maxSize, MaxSize);
		HL_DEFINE_GETSET(float, minScale, MinScale);
		HL_DEFINE_GETSET(float, maxScale, MaxScale);
		HL_DEFINE_GETSET(float, minSpeed, MinSpeed);
		HL_DEFINE_GETSET(float, maxSpeed, MaxSpeed);
		HL_DEFINE_GETSET(float, minAngle, MinAngle);
		HL_DEFINE_GETSET(float, maxAngle, MaxAngle);
		HL_DEFINE_SET(float, emissionRate, EmissionRate);
		HL_DEFINE_GETSET(Type, type, Type);
		HL_DEFINE_GETSET(april::BlendMode, blendMode, BlendMode);
		// TODO - change access through textureFilename
		HL_DEFINE_GETSET(april::Texture*, texture, Texture);
		HL_DEFINE_GET(harray<Affector*>, affectors, Affectors);
		void setParticleLimit(int value);
		void setLife(float value);
		void setSize(gvec2 value);
		void setScale(float value);
		void setSpeed(float value);
		void setAngle(float value);

		void setLifeRange(float min, float max);
		void setSizeRange(gvec2 min, gvec2 max);
		void setScaleRange(float min, float max);
		void setSpeedRange(float min, float max);
		void setAngleRange(float min, float max);

		void addAffector(Affector* affector);
		void removeAffector(Affector* affector);
		void registerAffector(Affector* affector);
		void unregisterAffector(Affector* affector);
		void registerTexture(april::Texture* texture);
		void unregisterTexture(april::Texture* texture);
		Affector* getAffector(chstr name);
		template <class T>
		T* getAffector(chstr name)
		{
			return dynamic_cast<T*>(this->getAffector(name));
		}

		void update(float k);
		
		void draw(gvec3 point, gvec3 up, gvec3 offset = gvec3());
		void drawAffectors(); // usually only used for debug purposes
		
	protected:
		hstr name;
		float timer;
		gvec3 dimensions;
		float minLife;
		float maxLife;
		gvec2 minSize;
		gvec2 maxSize;
		float minScale;
		float maxScale;
		float minSpeed;
		float maxSpeed;
		float minAngle;
		float maxAngle;
		float emissionRate;
		int particleLimit;
		Type type;
		april::BlendMode blendMode;
		april::Texture* texture;
		april::Texture* registeredTexture;
		hdeque<Particle*> particles;
		harray<Affector*> affectors;
		harray<Affector*> registeredAffectors;

		void _createNewParticle();

	private:
		april::ColoredTexturedVertex* _triangleBatch;
		gvec3 _pos;
		float _rho;
		float _phi;
		float _theta;
		float _S;
		float _cs;
		int _quota;
		int _alive;
		Particle* _particle;
		gmat4 _billboard;
		gmat3 _rot;
		int _i;
		unsigned int _color;
		
		void _setupTriangleBatch();

	};
}

#endif // PARTICLE_EMITTER_H
