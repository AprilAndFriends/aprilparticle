#include "april/RenderSystem.h"
#include "april/Timer.h"
#include "aprilparticle/ParticleSystem.h"

#include "gtypes/Matrix3.h"
#include "gtypes/Vector3.h"

#include <iostream>

April::ColoredVertex grid[44];
April::ParticleSystem *mPartSys;

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
	
	drawGrid();
	return true;
}

int main()
{
	April::init("April",800,600,0,"AprilParticle Demo");
	April::rendersys->registerUpdateCallback(render);
	
	setupGrid(2.0);
	
	mPartSys = new April::ParticleSystem();
	mPartSys->loadParticleObject("../media/particle.apd");
    
	April::rendersys->enterMainLoop();
    
	April::destroy();
	return 0;
}
