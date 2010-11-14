#include "april/RenderSystem.h"
#include "aprilparticle/ParticleEmitter.h"
#include "april/Timer.h"
#include "aprilutil/StaticMesh.h"
#include "math.h"

#include "gtypes/Matrix3.h"
#include "gtypes/Vector3.h"

#include <iostream>

April::ParticleEmitter *emiter;
April::Texture *tex;
April::StaticMesh *msh;

April::Affectors::DirectionalForceField force1(4.0, gvec3(0,-1,0), gvec3(0,1,0));
April::Affectors::ColorAffector colorFader(0xFF1131FF, 0x00220000);
April::Affectors::MultiColorAffector colorAffector;
April::Affectors::LinearDirectionalForce linearforce1(gvec3(0,1,0), 0.012);
April::Affectors::Rotator rotator(50.0);
April::Affectors::Attractor attractor1(gvec3(0,0,0), 4.0);
April::Affectors::Swirl swirl(gvec3(0,0,0), gvec3(0,1,0), 12.0, 1.0);


April::ColoredVertex grid[44];

void setupGrid(float spacing)
{
	int i = 0;
	for(float s = -5*spacing; s <= 5*spacing; ++i, s += spacing)
	{
		April::ColoredVertex u,v,p,r;
		u.color = v.color = p.color = r.color = 0xFFAAAAAA;
		u.x = -5*spacing;
		u.y = 0;
		u.z = s;
		v.x = 5*spacing;
		v.y = 0;
		v.z = s;
		grid[i*4 + 0] = u;
		grid[i*4 + 1] = v;
		
		p.x = s;
		p.y = 0;
		p.z = -5*spacing;
		r.x = s;
		r.y = 0;
		r.z = 5*spacing;
		grid[i*4 + 2] = p;
		grid[i*4 + 3] = r;
	}
}

void drawGrid()
{
	April::rendersys->render(April::LineList, grid, 44);
}

bool render(float time_increase)
{
	April::rendersys->clear(true, false);
	static float angle=0;
	angle+=time_increase*90;
    
	April::rendersys->setPerspective(60,1024/768.,0.1f,100.0f);
	
	gvec3 pos(0,18,25);
	gmat3 rot;
	rot.setRotation3D(0,1,0, angle * 0.2);
	pos = rot * pos;
	April::rendersys->lookAt(pos,gtypes::Vector3(0,0,0),gtypes::Vector3(0,1,0));
	
	emiter->update(time_increase);
	
	emiter->draw(pos, gvec3(0,1,0));
	
	drawGrid();
	
	//emiter->drawAffectors();
	
	April::rendersys->setBlendMode(April::DEFAULT);
	April::rendersys->translate(0,0,5);
	April::rendersys->setTexture(tex);
	msh->draw(April::TriangleList);
	April::rendersys->translate(0,0,-5);
    
	return true;
}

int main()
{
	April::init("April",800,600,0,"AprilParticle Demo");
	April::rendersys->registerUpdateCallback(render);
	
	setupGrid(2.0);
	
	msh = new April::StaticMesh("../media/mesh.obj");
	tex = April::rendersys->loadTexture("../media/texture.jpg");
	
	emiter = new April::ParticleEmitter(4, 10);
	emiter->setMaxParticles(40);
	emiter->setParticleDrawType(April::PDT_Additive);
	emiter->setTexture("../media/partikl_strange.png");
	emiter->setEmitterType(April::ET_HollowSphere);
	emiter->setEmiterVolume(1,3,1);
	
	emiter->mPosition.x = 0;
	emiter->mPosition.y = 0;
	
	emiter->mRandomStartAngle = true;
	//emiter->mRandomStartSize = true;
	emiter->mSize = 5.2;
	emiter->mMinSize = 2.2;
	emiter->mMaxSize = 3.4;
	
	std::map<float, unsigned int> cols;
	cols[0.0]  = 0x00FF0000;
	cols[0.1] = 0xF3FF6432;
	cols[0.65] = 0xAF3F3F3F;
	cols[1.0]  = 0x00000000;
	
	colorAffector.setColors(cols);
	
	//emiter->addAffector(&force1);
	//emiter->addAffector(&colorFader);
	emiter->addAffector(&colorAffector);
	emiter->addAffector(&linearforce1);
	emiter->addAffector(&rotator);
	//emiter->addAffector(&attractor1);
	//emiter->addAffector(&swirl);
    
	April::rendersys->enterMainLoop();
    
	April::destroy();
	delete emiter;
	delete tex;
	delete msh;
	return 0;
}
