#include "april/RenderSystem.h"
#include "aprilparticle/ParticleEmitter.h"
#include "april/Timer.h"
#include "aprilutil/StaticMesh.h"
#include "math.h"

#include "gtypes/Matrix3.h"
#include "gtypes/Vector3.h"

April::ParticleEmitter flame(4, 10), bubbles(6, 50);

April::Affectors::DirectionalForceField directionalForce(4.0, gvec3(0,-1,0), gvec3(0,1,0));
April::Affectors::ColorAffector colorFader(0xFF1131FF, 0x00220000);
April::Affectors::MultiColorAffector colorAffector1, colorAffector2;
April::Affectors::LinearDirectionalForce linearforce1(gvec3(0,1,0), 18.0);
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
	drawGrid();
	
	flame.update(time_increase);
	flame.draw(pos, gvec3(0,1,0));
	
	bubbles.update(time_increase);
	bubbles.draw(pos, gvec3(0,1,0));
	
	
	return true;
}

void setupFlame()
{
	flame.setMaxParticles(40);
	flame.setParticleDrawType(April::PDT_Additive);
	flame.setTexture("../media/partikl_strange.png");
	flame.setEmitterType(April::ET_HollowSphere);
	flame.setEmiterVolume(1,3,1);
	flame.mPosition.x = -10;
	flame.mPosition.y = 0;
	flame.mRandomStartAngle = true;
	flame.mRandomStartSize = true;
	flame.mMinSize = 1.2;
	flame.mMaxSize = 3.4;
	flame.addAffector(&colorAffector1);
	flame.addAffector(&linearforce1);
	flame.addAffector(&rotator);
}

void setupBubbles()
{
	bubbles.setMaxParticles(300);
	bubbles.setParticleDrawType(April::PDT_Additive);
	bubbles.setTexture("../media/bubbles.png");
	bubbles.setEmitterType(April::ET_Cylinder);
	bubbles.setEmiterVolume(5,4,5);
	bubbles.mPosition.x = 0;
	bubbles.mPosition.y = 0;
	bubbles.mRandomStartSize = true;
	bubbles.mMinSize = 0.4;
	bubbles.mMaxSize = 1.1;
	bubbles.addAffector(&colorAffector2);
	bubbles.addAffector(&directionalForce);
}

int main()
{
	April::init("April",800,600,0,"AprilParticle Demo");
	April::rendersys->registerUpdateCallback(render);
	setupGrid(2.0);
	
	setupFlame();
	setupBubbles();
	
	std::map<float, unsigned int> cols1;
	cols1[0.0]  = 0x00FF0000;
	cols1[0.1] = 0xF3FF6432;
	cols1[0.65] = 0xAF3F3F3F;
	cols1[1.0]  = 0x00000000;
	
	colorAffector1.setColors(cols1);
	
	std::map<float, unsigned int> cols2;
	cols2[0.0]  = 0x00FFFFFF;
	cols2[0.1] = 0xFFFFFFF;
	cols2[0.6] = 0xFFFFFFF;
	cols2[1.0]  = 0x00FFFFFF;
	
	colorAffector2.setColors(cols2);
	
	
	//emiter->addAffector(&force1);
	//emiter->addAffector(&colorFader);
	
	//emiter->addAffector(&attractor1);
	//emiter->addAffector(&swirl);
    
	April::rendersys->enterMainLoop();
    
	April::destroy();
	
	return 0;
}
