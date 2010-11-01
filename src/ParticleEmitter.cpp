#include "ParticleEmitter.h"
#include "gtypes/Matrix4.h"
#include "gtypes/Matrix3.h"
#include "gtypes/Vector2.h"
#include <math.h>

namespace April
{

	ParticleEmitter::ParticleEmitter()
	{
		mDrawType 					= April::PDT_Additive;
		mEmitterType 				= April::ET_Point;
		
		mPosition					= gvec3(0.0,0.0,0.0);
		mDirection					= gvec3(0.0,1.0,0.0);
		
		mWidth = mHeigth = mLenght	= 1.0;
		
		mRandomStartAngle			= false;
		mRandomStartSize			= false;
		mRandomLife					= false;
		
		mSize						= 1.0;
		mMinSize					= 0.8;
		mMaxSize					= 1.2;
			
		mParticlesPerSecond			= 1.0;
		mCounter					= 0.0;
		mMaxParticles				= 10;
		mLife = mLifeMax = mLifeMin	= 1.0;
			
		mTex						= NULL;
	}
	
	ParticleEmitter::ParticleEmitter(float life, float particlesPerSecond, gvec3 position, gvec3 direction, unsigned int max)
	{
		mDrawType 					= April::PDT_Additive;
		mEmitterType 				= April::ET_Point;
		
		mPosition					= position;
		mDirection					= direction;
		
		mWidth = mHeigth = mLenght	= 1.0;
		
		mRandomStartAngle			= false;
		mRandomStartSize			= false;
		mRandomLife					= false;
		
		mSize						= 1.0;
		mMinSize					= 0.8;
		mMaxSize					= 1.2;
			
		mParticlesPerSecond			= particlesPerSecond;
		mCounter					= 0.0;
		mMaxParticles				= max;
		mLife = mLifeMax = mLifeMin	= life;
			
		mTex						= NULL;
	}

	ParticleEmitter::~ParticleEmitter()
	{
	}
	
	void ParticleEmitter::setTexture(std::string texture)
	{
		mTex = April::rendersys->loadTexture(texture);
	}
	
	bool isDead(const April::Particle& particle)
	{
		return (particle.mLife < 0.0);
	}
	
	void ParticleEmitter::createParticle()
	{
		gvec3 npos;
			switch(mEmitterType)
			{
				case April::ET_Point:
				{
					npos = mPosition;
					break;
				}
				case April::ET_Sphere:
				{
					float rho, phi, theta, S;
					
					rho = ((float)(rand()) / RAND_MAX);
					phi = ((float)(rand()) / RAND_MAX) * 2 * M_PI;
					theta = ((float)(rand()) / RAND_MAX) * M_PI;
					
					S = rho * sin(phi);
					npos.x = mPosition.x + S * cos(theta) * mLenght * 0.5;
					npos.y = mPosition.y + S * sin(theta) * mHeigth * 0.5;
					npos.z = mPosition.z + rho * cos(phi) * mWidth * 0.5;
					break;
				}
				case April::ET_HollowSphere:
				{
					float rho, phi, theta, S;
					
					rho = 1.0;
					phi = ((float)(rand()) / RAND_MAX) * 2 * M_PI;
					theta = ((float)(rand()) / RAND_MAX) * M_PI;
					
					S = rho * sin(phi);
					npos.x = mPosition.x + S * cos(theta) * mLenght * 0.5;
					npos.y = mPosition.y + S * sin(theta) * mHeigth * 0.5;
					npos.z = mPosition.z + rho * cos(phi) * mWidth * 0.5;
					break;
				}
				case April::ET_Box:
				{
					npos.x = mPosition.x + ((float)(rand()) / RAND_MAX) * mLenght - mLenght * 0.5;
					npos.y = mPosition.y + ((float)(rand()) / RAND_MAX) * mHeigth - mHeigth * 0.5;
					npos.z = mPosition.z + ((float)(rand()) / RAND_MAX) * mWidth - mWidth * 0.5;
					break;
				}
				case April::ET_Cylinder:
				{
					float rho, phi, theta, S;
					
					rho = ((float)(rand()) / RAND_MAX);
					phi = ((float)(rand()) / RAND_MAX) * 2 * M_PI;
					theta = ((float)(rand()) / RAND_MAX) * M_PI;
					
					S = rho * sin(phi);
					
					npos.x = mPosition.x + S * cos(theta) * mLenght * 0.5;
					npos.y = mPosition.y + ((float)(rand()) / RAND_MAX) * mHeigth - mHeigth * 0.5;
					npos.z = mPosition.z + rho * cos(phi) * mWidth * 0.5;
					break;
				}
				case April::ET_HollowCylinder:
				{
					float rho, phi, theta, S;
					
					rho = 1.0;
					phi = ((float)(rand()) / RAND_MAX) * 2 * M_PI;
					theta = ((float)(rand()) / RAND_MAX) * M_PI;
					
					S = rho * sin(phi);
					
					npos.x = mPosition.x + S * cos(theta) * mLenght * 0.5;
					npos.y = mPosition.y + ((float)(rand()) / RAND_MAX) * mHeigth - mHeigth * 0.5;
					npos.z = mPosition.z + rho * cos(phi) * mWidth * 0.5;
					break;
				}
				case April::ET_Ring:
				{
					// 2do
					npos = mPosition;
					break;
				}
			}
			Particle p(mLife,npos,mDirection, 0.0);
			
			if(mRandomStartSize) p.mSize = mMinSize + (mMaxSize - mMinSize) * ((float)(rand()) / RAND_MAX);
			else p.mSize = mSize;
			
			if(mRandomStartAngle) p.mAngle = (float)(rand());
			
			mParticles.push_back(p);
	}
	
	void ParticleEmitter::update(float t)
	{
		mCounter += t;
		if(mParticlesPerSecond > 0)
		{
			float cs = 1.0 / mParticlesPerSecond;
			int quotta = (int)(mCounter / cs);
			if(mParticles.size() >= mMaxParticles)
				mCounter = 0.0;
			if((mCounter > cs) &&(mParticles.size() < mMaxParticles))
			{
				for(int i = 0; i < quotta; ++i)
					createParticle();
				mCounter = 0.0;
			}
		}
		
		for(std::list<April::Particle>::iterator it = mParticles.begin(); it != mParticles.end(); it++)
		{
			it->mLife -= t;
			for(std::list<Affectors::Affector*>::iterator jt = mAffecttors.begin(); jt != mAffecttors.end(); jt++)
			{
				(*jt)->update(&(*it), t);
			}
		}
		
		mParticles.remove_if(isDead);
		
	}
	
	void ParticleEmitter::addAffector(Affectors::Affector* affector)
	{
		mAffecttors.push_back(affector);
	}
	
	void ParticleEmitter::draw()
	{
		April::ColoredTexturedVertex v[4];
		gmat4 billboard, cam = April::rendersys->getModelviewMatrix();
		
		if(mTex!= NULL)
			April::rendersys->setTexture(mTex);
		
		for(std::list<April::Particle>::iterator it = mParticles.begin(); it != mParticles.end(); it++)
		{
			
			billboard.lookAt(cam * it->mPosition, 
							(gvec3(cam[12], cam[13], cam[14]) - cam * it->mPosition),
							gvec3(cam[4], cam[5], cam[6]));
			
			
			float s = it->mSize;
			
			gvec3 v0,v1,v2,v3;
			v0 = gvec3(-s/2, -s/2, 0.0);
			v1 = gvec3( s/2, -s/2, 0.0);
			v2 = gvec3(-s/2,  s/2, 0.0);
			v3 = gvec3( s/2,  s/2, 0.0);
			
			if(mRandomStartAngle)
			{
				gmat3 rot;
				rot.setRotation3D(0,0,1,it->mAngle);
				v0 = rot * v0;
				v1 = rot * v1;
				v2 = rot * v2;
				v3 = rot * v3;
			}
			
			/*billboard.inverse();
			v0 = billboard * v0;
			v1 = billboard * v1;
			v2 = billboard * v2;
			v3 = billboard * v3;*/
			
			v[0].x = v0.x; v[0].y =  v0.y; v[0].z = v0.z;   v[0].u = 0.0; v[0].v = 0.0; v[0].color = it->mColor;
			v[1].x = v1.x; v[1].y =  v1.y; v[1].z = v1.z;   v[1].u = 1.0; v[1].v = 0.0; v[1].color = it->mColor;
			v[2].x = v2.x; v[2].y =  v2.y; v[2].z = v2.z;   v[2].u = 0.0; v[2].v = 1.0; v[2].color = it->mColor;
			v[3].x = v3.x; v[3].y =  v3.y; v[3].z = v3.z;   v[3].u = 1.0; v[3].v = 1.0; v[3].color = it->mColor;
			
			April::rendersys->setModelviewMatrix(billboard.inversed());
			April::rendersys->render(April::TriangleStrip, v, 4);
			April::rendersys->setModelviewMatrix(cam);
			
		}
	}
	
	void ParticleEmitter::drawAffectors()
	{
		for(std::list<Affectors::Affector*>::iterator it = mAffecttors.begin(); it != mAffecttors.end(); it++)
			(*it)->draw();
	}
	
	
	void ParticleEmitter::setEmiterVolume(float width, float heigth, float lenght)
	{
		mWidth 		= width;
		mHeigth 	= heigth;
		mLenght 	= lenght;
		
	}
	
	void ParticleEmitter::setEmitterType(April::EmitterType type)
	{
		mEmitterType = type;
	}

}

