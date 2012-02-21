/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.4
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

#include <april/Color.h>
#include <april/RenderSystem.h>
#include <gtypes/Matrix3.h>
#include <gtypes/Matrix4.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/hdeque.h>
#include <hltypes/hlist.h>
#include <hltypes/hltypesUtil.h>

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
			HollowCylinder
		};
	
		friend class System;

		Emitter(chstr name = "");
		~Emitter();

		HL_DEFINE_ISSET(bool, running, Running);
		HL_DEFINE_GETSET(Type, type, Type);
		HL_DEFINE_GETSET(gvec3, dimensions, Dimensions);
		void setDimensions(float x, float y, float z) { this->dimensions.set(x, y, z); }
		HL_DEFINE_GETSET(april::BlendMode, blendMode, BlendMode);
		HL_DEFINE_GETSET(float, emissionRate, EmissionRate);
		HL_DEFINE_GETSET(float, duration, Duration);
		HL_DEFINE_GETSET(float, delay, Delay);
		HL_DEFINE_GETSET(float, loopDelay, LoopDelay);
		HL_DEFINE_GETSET(int, loops, Loops);
		HL_DEFINE_GET(int, limit, Limit);
		void setLimit(int value);
		HL_DEFINE_GETSET(float, preUpdate, PreUpdate);
		HL_DEFINE_GETSET(float, minLife, MinLife);
		HL_DEFINE_GETSET(float, maxLife, MaxLife);
		HL_DEFINE_GETSET(gvec3, minDirection, MinDirection);
		HL_DEFINE_GETSET(gvec3, maxDirection, MaxDirection);
		HL_DEFINE_GETSET(gvec2, minSize, MinSize);
		HL_DEFINE_GETSET(gvec2, maxSize, MaxSize);
		HL_DEFINE_GETSET(float, minScale, MinScale);
		HL_DEFINE_GETSET(float, maxScale, MaxScale);
		HL_DEFINE_GETSET(float, minAngle, MinAngle);
		HL_DEFINE_GETSET(float, maxAngle, MaxAngle);
		HL_DEFINE_GETSET(april::Texture*, texture, Texture);
		HL_DEFINE_GET(harray<Affector*>, affectors, Affectors);
		void setLife(float value);
		void setDirection(gvec3 value);
		void setSize(gvec2 value);
		void setScale(float value);
		void setAngle(float value);
		void setLife(chstr value);
		void setDirection(chstr value);
		void setSize(chstr value);
		void setScale(chstr value);
		void setAngle(chstr value);
		int getParticleCount();
		bool isExpired();

		void setLifeRange(float min, float max);
		void setDirectionRange(gvec3 min, gvec3 max);
		void setSizeRange(gvec2 min, gvec2 max);
		void setScaleRange(float min, float max);
		void setAngleRange(float min, float max);

		void addAffector(Affector* affector);
		void removeAffector(Affector* affector);

		hstr getProperty(chstr name, bool* property_exists = NULL);
		bool setProperty(chstr name, chstr value);

		void reset();
		void update(float k);
		
		void draw(gvec3 point, gvec3 up); // is the only 3D drawing method
		void draw(gvec2 offset = gvec2());
		void draw(gvec2 offset, april::Color color);
		void drawAffectors(); // usually only used for debug purposes, 3D
		
	protected:
		float emissionTimer;
		float loopTimer;
		float time;
		bool running;
		Type type;
		gvec3 dimensions;
		april::BlendMode blendMode;
		float emissionRate;
		float duration;
		float delay;
		float loopDelay;
		int loops;
		int currentLoop;
		int alive;
		int limit;
		float preUpdate;
		float minLife;
		float maxLife;
		gvec3 minDirection;
		gvec3 maxDirection;
		gvec2 minSize;
		gvec2 maxSize;
		float minScale;
		float maxScale;
		float minAngle;
		float maxAngle;
		april::Texture* texture;
		harray<Affector*> affectors;
		hdeque<Particle*> particles;
		System* system;
		bool started;

		void _createNewParticle(float k);
		void _setSystem(System* value) { this->system = value; }

	private:
		april::ColoredTexturedVertex* _triangleBatch;
		gvec3 _movement;
		gvec3 _pos;
		float _rho;
		float _phi;
		float _theta;
		float _angle;
		float _cs;
		int _quota;
		bool _expired;
		Particle* _particle;
		gmat4 _billboard;
		gmat3 _rot;
		gvec3 _offset;
		float _xSize;
		float _ySize;
		int _i;
		float _w;
		float _h;
		unsigned int _color;
		
		void _setupTriangleBatch();

	};
}

#endif // PARTICLE_EMITTER_H
