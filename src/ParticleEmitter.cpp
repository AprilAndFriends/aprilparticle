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
		_mTriangleBatch = new April::ColoredTexturedVertex[mMaxParticles * 6];
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
		_mTriangleBatch = new April::ColoredTexturedVertex[mMaxParticles * 6];
		mLife = mLifeMax = mLifeMin	= life;
			
		mTex						= NULL;
	}

	ParticleEmitter::~ParticleEmitter()
	{
		delete[] _mTriangleBatch;
	}
	
	void ParticleEmitter::setTexture(std::string texture)
	{
		mTex = April::rendersys->loadTexture(texture);
	}
	
	/*bool isDead(const April::Particle& particle)
	{
		if(particle.mLife < 0.0)
		{
			return true;
		}
		return false;
	}*/
	
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
		
		for(std::deque<April::Particle>::iterator it = mParticles.begin(); it != mParticles.end(); it++)
		{
			it->mLife -= t;
			for(std::list<Affectors::Affector*>::iterator jt = mAffecttors.begin(); jt != mAffecttors.end(); jt++)
			{
				(*jt)->update(&(*it), t);
			}
		}
		
		while(mParticles.front().mLife < 0.0)
			mParticles.pop_front();
		
	}
	
	void ParticleEmitter::addAffector(Affectors::Affector* affector)
	{
		mAffecttors.push_back(affector);
	}
	
	void ParticleEmitter::draw(gvec3 point, gvec3 up)
	{
		gmat4 billboard;
		gvec3 v0,v1,v2,v3;
		gmat3 rot;
		float s;
		April::ColoredTexturedVertex v[6];
		
		int i = 0;
		for(std::deque<April::Particle>::iterator it = mParticles.begin(); it != mParticles.end(); i++, it++)
		{
			billboard.lookAt(it->mPosition, point - it->mPosition, up);
			s = it->mSize;
			
			v0 = gvec3(-s/2, -s/2, 0.0);
			v1 = gvec3( s/2, -s/2, 0.0);
			v2 = gvec3(-s/2,  s/2, 0.0);
			v3 = gvec3( s/2,  s/2, 0.0);
			
			rot.setRotation3D(0,0,1,it->mAngle);
			v0 = rot * v0;
			v1 = rot * v1;
			v2 = rot * v2;
			v3 = rot * v3;
			
			billboard.inverse();
			v0 = billboard * v0;
			v1 = billboard * v1;
			v2 = billboard * v2;
			v3 = billboard * v3;
			
			v[0].x = v0.x; v[0].y =  v0.y; v[0].z = v0.z;   v[0].u = 1.0; v[0].v = 1.0; v[0].color = it->mColor;
			v[1].x = v1.x; v[1].y =  v1.y; v[1].z = v1.z;   v[1].u = 0.0; v[1].v = 1.0; v[1].color = it->mColor;
			v[2].x = v2.x; v[2].y =  v2.y; v[2].z = v2.z;   v[2].u = 1.0; v[2].v = 0.0; v[2].color = it->mColor;
			
			v[3].x = v1.x; v[3].y =  v1.y; v[3].z = v1.z;   v[3].u = 0.0; v[3].v = 1.0; v[3].color = it->mColor;
			v[4].x = v2.x; v[4].y =  v2.y; v[4].z = v2.z;   v[4].u = 1.0; v[4].v = 0.0; v[4].color = it->mColor;
			v[5].x = v3.x; v[5].y =  v3.y; v[5].z = v3.z;   v[5].u = 0.0; v[5].v = 0.0; v[5].color = it->mColor;
			
			_mTriangleBatch[i*6 + 0] = v[0];
			_mTriangleBatch[i*6 + 1] = v[1];
			_mTriangleBatch[i*6 + 2] = v[2];
			_mTriangleBatch[i*6 + 3] = v[3];
			_mTriangleBatch[i*6 + 4] = v[4];
			_mTriangleBatch[i*6 + 5] = v[5];
			
		}
		
		if(mTex != NULL)
			April::rendersys->setTexture(mTex);
		
		switch(mDrawType)
		{
			case April::PDT_Additive:
				April::rendersys->setBlendMode(April::ADD);
				break;
			case April::PDT_Normal:
				April::rendersys->setBlendMode(April::DEFAULT);
				break;
			case April::PDT_AlphaBlend:
				April::rendersys->setBlendMode(April::ALPHA_BLEND);
				break;
		}
		April::rendersys->render(April::TriangleList, _mTriangleBatch, mParticles.size() * 6);
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
	
	void ParticleEmitter::setMaxParticles(int maxParticles)
	{
		mMaxParticles = maxParticles;
		delete[] _mTriangleBatch;
		_mTriangleBatch = new April::ColoredTexturedVertex[maxParticles * 6];
	}

}

