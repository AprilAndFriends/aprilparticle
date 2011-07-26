#include <april/main.h>
#include <april/RenderSystem.h>
#include <april/Timer.h>
#include <april/Window.h>
//#include <aprilparticle/ParticleSystem.h>
#include <gtypes/Matrix3.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include <iostream>

april::ColoredVertex grid[44];
//april::ParticleSystem *mPartSys;

void setupGrid(float spacing)
{
	int i = 0;
	for(float s = -5*spacing; s <= 5*spacing; ++i, s += spacing)
	{
		april::ColoredVertex u,v,p,r;
		u.color = v.color = p.color = r.color = 0xAAAAAAFF;
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
	april::rendersys->render(april::LineList, grid, 44);
}

bool render(float time_increase)
{
	april::rendersys->clear(true, false);
	static float angle = 0.0f;
	angle += time_increase * 90.0f;
    
	april::rendersys->setPerspective(60.0f, 1024.0f / 768.0f, 0.1f, 100.0f);
	
	gvec3 pos(0.0f, 18.0f, 25.0f);
	gmat3 rot;
	rot.setRotation3D(0.0f, 1.0f, 0.0f, angle * 0.2f);
	pos = rot * pos;
	april::rendersys->lookAt(pos, gvec3(0.0f, 0.0f, 0.0f), gvec3(0.0f, 1.0f, 0.0f));
	
	drawGrid();
	return true;
}

void april_init(const harray<hstr>& args)
{
	april::init();
	april::createRenderSystem("");
	april::createRenderTarget(800, 600, false, "AprilParticle Demo");
	april::rendersys->getWindow()->setUpdateCallback(render);
	
	setupGrid(2.0f);
	
	//mPartSys = new april::ParticleSystem();
	//mPartSys->loadParticleObject("../media/particle.apd");
    
}

void april_destroy()
{
	april::destroy();
}