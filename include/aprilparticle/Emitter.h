/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.3
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
#include "ActiveObject.h"

namespace april
{
	class Texture;
}

namespace aprilparticle
{
	class Affector;
	class Particle;
	class System;

	class aprilparticleExport Emitter : public ActiveObject
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
	
		friend class System;

		Emitter(chstr name = "");
		~Emitter();

		HL_DEFINE_GETSET(Type, type, Type);
		HL_DEFINE_GETSET(gvec3, dimensions, Dimensions);
		void setDimensions(float x, float y, float z) { this->dimensions.set(x, y, z); }
		HL_DEFINE_GETSET(april::BlendMode, blendMode, BlendMode);
		HL_DEFINE_SET(float, emissionRate, EmissionRate);
		void setLimit(int value);
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
		// TODO - change access through textureFilename
		HL_DEFINE_GETSET(april::Texture*, texture, Texture);
		HL_DEFINE_GET(harray<Affector*>, affectors, Affectors);
		void setLife(float value);
		void setSize(gvec2 value);
		void setScale(float value);
		void setSpeed(float value);
		void setAngle(float value);
		void setLife(chstr value);
		void setSize(chstr value);
		void setScale(chstr value);
		void setSpeed(chstr value);
		void setAngle(chstr value);

		void setLifeRange(float min, float max);
		void setSizeRange(gvec2 min, gvec2 max);
		void setScaleRange(float min, float max);
		void setSpeedRange(float min, float max);
		void setAngleRange(float min, float max);

		void addAffector(Affector* affector);
		void removeAffector(Affector* affector);

		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);
		void update(float k);
		
		void draw(gvec3 point, gvec3 up);
		void draw2D();
		void drawAffectors(); // usually only used for debug purposes
		
	protected:
		float timer;
		Type type;
		gvec3 dimensions;
		april::BlendMode blendMode;
		float emissionRate;
		int limit;
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
		april::Texture* texture;
		harray<Affector*> affectors;
		hdeque<Particle*> particles;
		System* system;

		void _createNewParticle();
		void _setSystem(System* value) { this->system = value; }

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
		float _w;
		float _h;
		unsigned int _color;
		
		void _setupTriangleBatch();

	};
}

#endif // PARTICLE_EMITTER_H
