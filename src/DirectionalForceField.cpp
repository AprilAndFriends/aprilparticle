#include <math.h>

#include <april/RenderSystem.h>
#include <gtypes/Vector3.h>

#include "DirectionalForceField.h"
#include "Particle.h"

#define VERTEX_COUNT 91 // you can't touch this

namespace aprilparticle
{
	// optimizations
	april::ColoredVertex u[VERTEX_COUNT];
	april::ColoredVertex v[VERTEX_COUNT];
	april::ColoredVertex w[VERTEX_COUNT];
	april::ColoredVertex arrow[2];
	gvec3 ut;
	gvec3 vt;
	gvec3 wt;

	void initDirectionalForceField()
	{
		for (int i = 0; i < VERTEX_COUNT; i++)
		{
			ut.x = sin(i * 0.069777f);
			ut.y = cos(i * 0.069777f);
			vt.y = cos(i * 0.069777f);
			vt.z = sin(i * 0.069777f);
			wt.x = cos(i * 0.069777f);
			wt.z = sin(i * 0.069777f);
			u[i].color = 0xFFFFFFFF;
			v[i].color = 0xFFFFFFFF;
			w[i].color = 0xFFFFFFFF;
		}
		arrow[0].color = 0xFFFFFFFF;
		arrow[1].color = 0xFFFFFFFF;
	}

	DirectionalForceField::DirectionalForceField()
	{
		this->position = gvec3(0.0f, 0.0f, 0.0f);
		this->direction = gvec3(0.0f, 0.0f, 1.0f);
		this->force = 0.0f;
	}
		
	DirectionalForceField::DirectionalForceField(gvec3 position, gvec3 direction, float force)
	{
		this->position = position;
		this->direction = direction;
		this->force = force;
	}

	DirectionalForceField::~DirectionalForceField()
	{
	}
		
	void DirectionalForceField::update(Particle* particle, float k)
	{
		gvec3 difference = particle->position - this->position;
		float len = difference.length();
		particle->position += this->direction * (this->force - len / this->force) *
			(this->force / (len * len * particle->direction.length() + 1.0f) - particle->speed) * k;
	}
		
	void DirectionalForceField::draw()
	{
		for (int i = 0; i < VERTEX_COUNT; i++)
		{
			u[i] = this->position + ut * this->force;
			v[i] = this->position + vt * this->force;
			w[i] = this->position + wt * this->force;
		}
		arrow[0] = this->position;
		arrow[1] = this->position + this->direction * this->force;
		april::rendersys->render(april::LineStrip, u, VERTEX_COUNT);
		april::rendersys->render(april::LineStrip, v, VERTEX_COUNT);
		april::rendersys->render(april::LineStrip, w, VERTEX_COUNT);
		april::rendersys->render(april::LineStrip, arrow, 2);
	}

}

