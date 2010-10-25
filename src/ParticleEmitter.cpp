#include "ParticleEmitter.h"
#include "gtypes/Matrix4.h"

namespace April
{

	ParticleEmitter::ParticleEmitter()
	{
		this->life = 1.0;
		this->size = 0.5;
		
		this->randomSize = false;
		this->sizemin = 0;
		this->sizemax = 1;
		this->maxParticles = 256;
		
		this->position = gvec3(0,0,0);
		this->direction = gvec3(0,0,1);
		
		this->particleDecaySpeed = 1.0;
		this->creationSpeed = 0.1;
		this->speed = 0.1;
		this->counter = 0;
		this->tex = NULL;
	}
	
	ParticleEmitter::ParticleEmitter(float life, float size, float creationSpeed, float speed, gvec3 pos, gvec3 direction,
										bool rand, float smin, float smax, unsigned int max, float decay)
	{
		this->life = life;
		this->size = size;
		
		this->randomSize = rand;
		this->sizemin = smin;
		this->sizemax = smax;
		this->maxParticles = max;
		
		this->position = pos;
		this->direction = direction;
		
		this->particleDecaySpeed = decay;
		this->creationSpeed = creationSpeed;
		this->speed = speed;
		this->counter = 0;
		this->tex = NULL;
	}

	ParticleEmitter::~ParticleEmitter()
	{
	}
	
	void ParticleEmitter::setTexture(std::string texture)
	{
		tex = April::rendersys->loadTexture(texture);
	}
	
	bool isDead(const April::ParticleEmitter& particle)
	{
		return (particle.life < 0.0);
	}
	
	void ParticleEmitter::update(float t)
	{
		counter += t;
		if((counter > creationSpeed) && (particles.size() <= maxParticles))
		{
			particles.push_back(ParticleEmitter(life,size,creationSpeed,speed,position,direction,sizemin,sizemax,maxParticles,particleDecaySpeed));
			counter = 0.0;
		}
		
		for(std::list<April::ParticleEmitter>::iterator it = particles.begin(); it != particles.end(); it++)
		{
			it->life -= particleDecaySpeed*t;
			for(std::list<BaseAffector*>::iterator jt = affecttors.begin();
				jt != affecttors.end(); jt++)
			{
				(*jt)->update(&(*it), t);
			}
		}
		
		particles.remove_if(isDead);
		
	}
	
	void ParticleEmitter::addAffctor(BaseAffector* affector)
	{
		affecttors.push_back(affector);
	}
	
	void ParticleEmitter::draw()
	{
		//April::ColoredTexturedVertex v[4];
		April::TexturedVertex v[4];
		gmat4 cam = April::rendersys->getModelviewMatrix();
		gmat4 billboard;
		gvec3 up, left, facing;
		if(tex!= NULL)
			April::rendersys->setTexture(tex);
		for(std::list<April::ParticleEmitter>::iterator it = particles.begin(); it != particles.end(); it++)
		{
			facing = gvec3(cam[3], cam[7], cam[11]) - it->position;
			up = gvec3(cam[2], cam[6], cam[10]);
			left = up.cross(facing);
			up = left.cross(facing);
			billboard[0] = left.x; billboard[1] = up.x; billboard[2] = facing.x;
			billboard[4] = left.y; billboard[5] = up.y; billboard[6] = facing.y;
			billboard[8] = left.z; billboard[9] = up.z; billboard[10] = facing.z;
			billboard[15] = 1;
			billboard.setTranslation(it->position);
			
			gvec3 a,b,c,d;
			a = gvec3(-size/2, -size/2, 0);
			b = gvec3( size/2, -size/2, 0);
			c = gvec3(-size/2,  size/2, 0);
			d = gvec3( size/2,  size/2, 0);
			
			a = billboard * a;
			b = billboard * b;
			c = billboard * c;
			d = billboard * d;
			
			v[0].x= a.x;	v[0].y= a.y;	v[0].z= a.z;	v[0].u=0;	v[0].v=0;
			v[1].x= b.x;	v[1].y= b.y;	v[1].z= b.z;	v[1].u=1;	v[1].v=0;
			v[2].x= c.x;	v[2].y= c.y;	v[2].z= c.z;	v[2].u=0;	v[2].v=1;
			v[3].x= d.x;	v[3].y= d.y;	v[3].z= d.z;	v[3].u=1;	v[3].v=1;
			
			April::rendersys->setModelviewMatrix(cam);
			April::rendersys->render(April::TriangleStrip,v,4);
			
		}
	}

}

