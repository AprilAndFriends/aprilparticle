#include <math.h>

#include <april/RenderSystem.h>
#include <gtypes/Vector3.h>

#include "DirectionalForceField.h"
#include "Particle.h"

namespace april
{
	namespace Affectors
	{
		// optimizations
		april::ColoredVertex u[91];
		april::ColoredVertex v[91];
		april::ColoredVertex w[91];
		april::ColoredVertex arrow[2];

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
			gvec3 position = particle->getPosition();
			gvec3 difference = position - this->position;
			position += this->direction * (this->force - (difference.length()) / this->force) *
						(this->force / (difference.squaredLength() *
						particle->getDirection().length() + 1.0f) -
						particle->getSpeed()) * k;
			particle->setPosition(position);
		}
		
		void DirectionalForceField::draw()
		{
			for (int i = 0; i < 91; i++)
			{
				gvec3 u1(sin(i * 0.069777f), cos(i * 0.069777f), 0.0f);
				gvec3 v1(0.0f, cos(i * 0.069777f), sin(i * 0.069777f));
				gvec3 w1(cos(i * 0.069777f), 0.0f, sin(i * 0.069777f));

				u[i] = this->position + u1 * this->force;
				u[i].color = 0xFFFFFFFF;
				v[i] = this->position + v1 * this->force;
				v[i].color = 0xFFFFFFFF;
				w[i] = this->position + w1 * this->force;
				w[i].color = 0xFFFFFFFF;
			}
			arrow[0] = this->position;
			arrow[0].color = 0xFFFFFFFF;
			arrow[1] = this->position + this->direction * this->force;
			arrow[1].color = 0xFFFFFFFF;
			april::rendersys->render(april::LineStrip, u, 91);
			april::rendersys->render(april::LineStrip, v, 91);
			april::rendersys->render(april::LineStrip, w, 91);
			april::rendersys->render(april::LineStrip, arrow, 2);
		}
		
	}

}

