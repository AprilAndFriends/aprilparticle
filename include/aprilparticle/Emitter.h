/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
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
#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>

#include "aprilparticleExport.h"
#include "SpaceObject.h"

namespace april
{
	class Texture;
}

namespace aprilparticle
{
	class Affector;
	class Particle;
	class Space;

	class aprilparticleExport Emitter : public SpaceObject
	{
	public:
		HL_ENUM_CLASS_PREFIX_DECLARE(aprilparticleExport, Type,
		(
			HL_ENUM_DECLARE(Type, Point);
			HL_ENUM_DECLARE(Type, Box);
			HL_ENUM_DECLARE(Type, HollowBox);
			HL_ENUM_DECLARE(Type, Sphere);
			HL_ENUM_DECLARE(Type, HollowSphere);
			HL_ENUM_DECLARE(Type, Cylinder);
			HL_ENUM_DECLARE(Type, HollowCylinder);
			HL_ENUM_DECLARE(Type, Circle);
			HL_ENUM_DECLARE(Type, HollowCircle);
		));

		friend class Space;

		Emitter(chstr name = "");
		Emitter(const Emitter& other);
		~Emitter();

		hmap<hstr, PropertyDescription>& getPropertyDescriptions() const;

		HL_DEFINE_ISSET(running, Running);
		HL_DEFINE_GETSET(Type, type, Type);
		HL_DEFINE_GETSET(gvec3f, dimensions, Dimensions);
		inline void setDimensions(float x, float y, float z) { this->dimensions.set(x, y, z); }
		HL_DEFINE_GETSET(april::Color, color, Color);
		void setSymbolicColor(chstr value);
		HL_DEFINE_GETSET(april::BlendMode, blendMode, BlendMode);
		HL_DEFINE_GETSET(april::ColorMode, colorMode, ColorMode);
		HL_DEFINE_GETSET(float, colorModeFactor, ColorModeFactor);
		HL_DEFINE_GETSET(float, emissionRate, EmissionRate);
		HL_DEFINE_GETSET(float, duration, Duration);
		HL_DEFINE_GETSET(float, delay, Delay);
		HL_DEFINE_GETSET(float, loopDelay, LoopDelay);
		HL_DEFINE_GETSET(int, loops, Loops);
		HL_DEFINE_GET(int, limit, Limit);
		HL_DEFINE_ISSET(angleAligned, AngleAligned);
		HL_DEFINE_ISSET(directionAligned, DirectionAligned);
		void setLimit(int value);
		HL_DEFINE_ISSET(reverseRendering, ReverseRendering);
		HL_DEFINE_GETSET(float, minLife, MinLife);
		HL_DEFINE_GETSET(float, maxLife, MaxLife);
		HL_DEFINE_GETSET(gvec3f, minDirection, MinDirection);
		HL_DEFINE_GETSET(gvec3f, maxDirection, MaxDirection);
		HL_DEFINE_GETSET(gvec2f, minSize, MinSize);
		HL_DEFINE_GETSET(gvec2f, maxSize, MaxSize);
		HL_DEFINE_GETSET(float, minScale, MinScale);
		HL_DEFINE_GETSET(float, maxScale, MaxScale);
		HL_DEFINE_GETSET(float, minAngle, MinAngle);
		HL_DEFINE_GETSET(float, maxAngle, MaxAngle);
		HL_DEFINE_GETSET(april::Texture*, texture, Texture);
		void setLife(float value);
		void setDirection(cgvec3f value);
		void setSize(cgvec2f value);
		void setScale(float value);
		void setAngle(float value);
		void setLife(chstr value);
		void setDirection(chstr value);
		void setSize(chstr value);
		void setScale(chstr value);
		void setAngle(chstr value);
		bool isExpired() const;

		void setLifeRange(float min, float max);
		void setDirectionRange(cgvec3f min, cgvec3f max);
		void setSizeRange(cgvec2f min, cgvec2f max);
		void setScaleRange(float min, float max);
		void setAngleRange(float min, float max);

		hstr getProperty(chstr name);
		bool setProperty(chstr name, chstr value);

		void reset();
		void clearParticles();
		void update(float timeDelta);

		/// @note Not thread-safe!
		void draw(cgvec3f point, cgvec3f up); // the only 3D drawing method
		/// @note Not thread-safe!
		void draw(cgvec2f offset = gvec2f());
		/// @note Not thread-safe!
		void draw(cgvec2f offset, const april::Color& color);

	protected:
		float emissionTimer;
		float loopTimer;
		float time;
		bool running;
		Type type;
		gvec3f dimensions;
		april::Color color;
		april::BlendMode blendMode;
		april::ColorMode colorMode;
		float colorModeFactor;
		float emissionRate;
		float duration;
		float delay;
		float loopDelay;
		int loops;
		int currentLoop;
		int alive;
		int limit;
		bool angleAligned;
		bool directionAligned;
		bool reverseRendering;
		float minLife;
		float maxLife;
		gvec3f minDirection;
		gvec3f maxDirection;
		gvec2f minSize;
		gvec2f maxSize;
		float minScale;
		float maxScale;
		float minAngle;
		float maxAngle;
		april::Texture* texture;
		harray<Particle*> particles;

		void _createNewParticle(float timeDelta);
		inline void _setSpace(Space* value) { this->_space = value; }

	private:
		static hmap<hstr, PropertyDescription> _propertyDescriptions;

		Space* _space;
		april::ColoredTexturedVertex* _triangleBatch;
		gvec3f _pos;
		float _rho;
		float _phi;
		float _theta;
		float _angle;
		float _emissionTime;
		int _quota;
		int _fullQuota;
		bool _expired;
		gmat4 _billboard;
		gmat3 _rot;
		gvec3f _offset;
		float _xSize;
		float _ySize;
		int _i;
		float _w;
		float _h;
		unsigned int _color;
		int _pStart;
		int _pEnd;
		int _pStep;
		int _pI;

		void _setupTriangleBatch();

	};
}

#endif
