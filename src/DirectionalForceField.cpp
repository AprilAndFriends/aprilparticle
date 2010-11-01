#include "DirectionalForceField.h"
#include "April/RenderSystem.h"
#include <math.h>

namespace April
{

	namespace Affectors
	{
		
		DirectionalForceField::DirectionalForceField()
		{
			mForce = 0;
			mPosition = gvec3(0,0,0);
			mDirection = gvec3(0,0,1);
		}
		
		DirectionalForceField::DirectionalForceField(float force, gvec3 position, gvec3 direction)
		{
			mForce = force;
			mPosition = position;
			mDirection = direction;
		}

		DirectionalForceField::~DirectionalForceField()
		{
		}
		
		void DirectionalForceField::update(Particle *particle, double t)
		{
			particle->mPosition += mDirection * (mForce - ((particle->mPosition - mPosition).length()) / mForce) *
								  (mForce / (gvec3(particle->mPosition - mPosition).squaredLength() *
								  particle->mDirection.length() + 1.0) -
								  particle->mSpeed) * t;
			
		}
		
		void DirectionalForceField::draw()
		{
			April::ColoredVertex u[91], v[91], w[91];
			for(int i = 0; i < 91; ++i)
			{
				gvec3 u1(sin(i*0.069777), cos(i*0.069777), 0);
				gvec3 v1(0, cos(i*0.069777), sin(i*0.069777));
				gvec3 w1(cos(i*0.069777), 0, sin(i*0.069777));
				u[i].x = mPosition.x + u1.x*mForce;
				u[i].y = mPosition.y + u1.y*mForce;
				u[i].z = mPosition.z + u1.z*mForce; u[i].color = 0xFFFFFFFF;
				
				v[i].x = mPosition.x + v1.x*mForce;
				v[i].y = mPosition.y + v1.y*mForce;
				v[i].z = mPosition.z + v1.z*mForce; v[i].color = 0xFFFFFFFF;
				
				w[i].x = mPosition.x + w1.x*mForce;
				w[i].y = mPosition.y + w1.y*mForce;
				w[i].z = mPosition.z + w1.z*mForce; w[i].color = 0xFFFFFFFF;
			}
			
			April::ColoredVertex arrow[2];
			arrow[0].x = mPosition.x;
			arrow[0].y = mPosition.y;
			arrow[0].z = mPosition.z; arrow[0].color = 0xFFFFFFFF;
			
			
			arrow[1].x = mPosition.x + mDirection.x*mForce;
			arrow[1].y = mPosition.y + mDirection.y*mForce;
			arrow[1].z = mPosition.z + mDirection.z*mForce; arrow[1].color = 0xFFFFFFFF;
			
			April::rendersys->render(April::LineStrip,u,91);
			April::rendersys->render(April::LineStrip,v,91);
			April::rendersys->render(April::LineStrip,w,91);
			April::rendersys->render(April::LineStrip,arrow,2);
		}
		
	}

}

