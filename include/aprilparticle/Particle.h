#ifndef PARTICLE_H
#define PARTICLE_H

#include <gtypes/Vector3.h>

#include "aprilparticleExport.h"

namespace april
{
	class aprilparticleExport Particle
	{
	public:
		Particle();
		Particle(float life, gvec3 position, gvec3 direction, float size);
		virtual ~Particle();

		gvec3 getPosition() { return this->position; }
		void setPosition(gvec3 value) { this->position = value; }
		gvec3 getDirection() { return this->direction; }
		void setDirection(gvec3 value) { this->direction = value; }
		float getLife() { return this->life; }
		void setLife(float value) { this->life = value; }
		float getTotalLife() { return this->totalLife; }
		void setTotalLife(float value) { this->totalLife = value; }
		float getSize() { return this->size; }
		void setSize(float value) { this->size = value; }
		float getSpeed() { return this->speed; }
		void setSpeed(float value) { this->speed = value; }
		float getAngle() { return this->angle; }
		void setAngle(float value) { this->angle = value; }
		unsigned int getColor() { return this->color; }
		void setColor(unsigned int value) { this->color = value; }

	protected:
		gvec3 position;
		gvec3 direction;
		float life;
		float totalLife;
		float size;
		float speed;
		float angle;
		unsigned int color;

	};

}

#endif // _PARTICLE_H_
