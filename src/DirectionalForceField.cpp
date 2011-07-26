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
		
		DirectionalForceField::DirectionalForceField(float force, gvec3 position, gvec3 direction)
		{
			this->position = position;
			this->direction = direction;
			this->force = force;
		}

		DirectionalForceField::~DirectionalForceField()
		{
		}
		
		void DirectionalForceField::update(Particle* particle, double t)
		{
			gvec3 position = particle->getPosition();
			gvec3 difference = position - this->position;
			position += this->direction * (this->force - (difference.length()) / this->force) *
						(this->force / (difference.squaredLength() *
						particle->getDirection().length() + 1.0f) -
						particle->getSpeed()) * (float)t;
			particle->setPosition(position);
		}
		
		void DirectionalForceField::draw()
		{
			for (int i = 0; i < 91; i++)
			{
				gvec3 u1((float)sin(i * 0.069777), (float)cos(i * 0.069777), 0.0f);
				gvec3 v1(0.0f, (float)cos(i * 0.069777), (float)sin(i * 0.069777));
				gvec3 w1((float)cos(i * 0.069777), 0.0f, (float)sin(i * 0.069777));

				u[i].x = this->position.x + u1.x * this->force;
				u[i].y = this->position.y + u1.y * this->force;
				u[i].z = this->position.z + u1.z * this->force;
				u[i].color = 0xFFFFFFFF;
				
				v[i].x = this->position.x + v1.x * this->force;
				v[i].y = this->position.y + v1.y * this->force;
				v[i].z = this->position.z + v1.z * this->force;
				v[i].color = 0xFFFFFFFF;
				
				w[i].x = this->position.x + w1.x * this->force;
				w[i].y = this->position.y + w1.y * this->force;
				w[i].z = this->position.z + w1.z * this->force;
				w[i].color = 0xFFFFFFFF;
			}
			
			arrow[0].x = this->position.x;
			arrow[0].y = this->position.y;
			arrow[0].z = this->position.z;
			arrow[0].color = 0xFFFFFFFF;
			
			arrow[1].x = this->position.x + this->direction.x * this->force;
			arrow[1].y = this->position.y + this->direction.y * this->force;
			arrow[1].z = this->position.z + this->direction.z * this->force;
			arrow[1].color = 0xFFFFFFFF;
			
			april::rendersys->render(april::LineStrip, u, 91);
			april::rendersys->render(april::LineStrip, v, 91);
			april::rendersys->render(april::LineStrip, w, 91);
			april::rendersys->render(april::LineStrip, arrow, 2);
		}
		
	}

}

