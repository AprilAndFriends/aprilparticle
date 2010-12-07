#include "april/RenderSystem.h"
#include "aprilparticle/ParticleEmitter.h"
#include "april/Timer.h"
#include "aprilutil/StaticMesh.h"
#include "math.h"

#include "gtypes/Matrix3.h"
#include "gtypes/Vector3.h"

April::ParticleEmitter flame(4, 32), bubbles(6, 50), vortex(2, 200);

April::Affectors::DirectionalForceField directionalForce(4.0, gvec3(0,1,0), gvec3(0,1,0));
April::Affectors::ColorAffector colorFader(0xFF1131FF, 0x00220000);
April::Affectors::MultiColorAffector colorAffector1, colorAffector2;
April::Affectors::LinearDirectionalForce linearforce1(gvec3(0,1,0), 18.0);
April::Affectors::Rotator rotator(50.0);
April::Affectors::Attractor attractor1(gvec3(10,0,0), 2.0);
April::Affectors::Swirl swirl(gvec3(10,0,0), gvec3(0,1,0), 8.0, 0.2);

April::Texture *jet, *plasmaball, *fire, *red, *raindrop, *boubbles;

// kvazar
April::ParticleEmitter quazar_disc(4, 240), quazar_jet_top(4, 120), quazar_jet_bottom(4, 120);
April::Affectors::LinearDirectionalForce top(gvec3(1,0,0), 24), bottom(gvec3(-1,0,0), 24);
April::Affectors::Attractor gravity(gvec3(0,0,10), 2.0);
April::Affectors::Swirl spin(gvec3(0,0,10), gvec3(1,0,0), 8.0, 0.1);
April::Affectors::Attractor inverse_gravitiy(gvec3(0,0,10), -4.0);
April::Affectors::Attractor attraction_point_bottom(gvec3(-10,0,10), 6.0);
April::Affectors::Attractor attraction_point_top(gvec3(10,0,10), 6.0);

// kisa
April::ParticleEmitter rain(4, 60);
April::Affectors::LinearDirectionalForce gravity_rain(gvec3(0,1,0), -12);

// twirl
April::ParticleEmitter twirl1(6, 100);
April::Affectors::DirectionalForceField force1(4.0, gvec3(10,0,-10), gvec3(-1,1,0));
April::Affectors::DirectionalForceField force2(4.0, gvec3(10,4,-10), gvec3(1,1,0));

April::ParticleEmitter twirl2(6, 100);
April::Affectors::DirectionalForceField force3(4.0, gvec3(10,0,-10), gvec3(-1,1,0));
April::Affectors::DirectionalForceField force4(4.0, gvec3(10,4,-10), gvec3(1,1,0));

April::Affectors::LinearDirectionalForce wind(gvec3(-1,0,0), 12.0);
April::Affectors::Attractor grav(gvec3(0, 0,-10), 7.0);


April::ColoredVertex grid[44];

void setupGrid(float spacing)
{
	int i = 0;
	for(float s = -5*spacing; s <= 5*spacing; ++i, s += spacing)
	{
		April::ColoredVertex u,v,p,r;
		u.color = v.color = p.color = r.color = 0xFF222222;
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
	
	vortex.update(time_increase);
	vortex.draw(pos, gvec3(0,1,0));
	
	// kvazar
	quazar_disc.update(time_increase);
	quazar_jet_bottom.update(time_increase);
	quazar_jet_top.update(time_increase);
	quazar_disc.draw(pos, gvec3(0,1,0));
	quazar_jet_bottom.draw(pos, gvec3(0,1,0));
	quazar_jet_top.draw(pos, gvec3(0,1,0));
	
	// rain
	rain.update(time_increase);
	rain.draw(pos, gvec3(0,1,0));
	
	force1.mDirection.x = sin(angle * 0.066);
	force1.mDirection.z = cos(angle * 0.033);
	
	force2.mDirection.y = sin(angle * 0.033);
	force2.mDirection.x = sin(angle * 0.066);
	
	force3.mDirection.z = sin(angle * 0.066);
	force3.mDirection.x = cos(angle * 0.033);
	
	force4.mDirection.x = sin(angle * 0.033);
	force4.mDirection.y = sin(angle * 0.066);
	
	twirl1.update(time_increase);
	twirl1.draw(pos, gvec3(0,1,0));
	
	twirl2.update(time_increase);
	twirl2.draw(pos, gvec3(0,1,0));
	
	
	return true;
}

void setupQuazar()
{
	quazar_disc.setMaxParticles(960);
	quazar_jet_top.setMaxParticles(480);
	quazar_jet_bottom.setMaxParticles(480);
	
	quazar_disc.setParticleDrawType(April::PDT_Additive);
	quazar_jet_top.setParticleDrawType(April::PDT_Additive);
	quazar_jet_bottom.setParticleDrawType(April::PDT_Additive);
	
	quazar_disc.setTexture(plasmaball);
	quazar_jet_bottom.setTexture(jet);
	quazar_jet_top.setTexture(jet);
	
	quazar_disc.setEmitterType(April::ET_HollowSphere);
	quazar_disc.setEmiterVolume(6,6,1);
	quazar_disc.mPosition.x = 0;
	quazar_disc.mPosition.z = 10;
	quazar_disc.mSize = 0.3;
	quazar_disc.addAffector(&gravity);
	quazar_disc.addAffector(&spin);
	
	quazar_jet_bottom.setEmitterType(April::ET_Sphere);
	quazar_jet_bottom.setEmiterVolume(1,1,1);
	quazar_jet_bottom.mPosition.x = 0;
	quazar_jet_bottom.mPosition.z = 10;
	quazar_jet_bottom.mSize = 0.3;
	quazar_jet_bottom.addAffector(&bottom);
	quazar_jet_bottom.addAffector(&inverse_gravitiy);
	quazar_jet_bottom.addAffector(&attraction_point_bottom);
	
	quazar_jet_top.setEmitterType(April::ET_Sphere);
	quazar_jet_top.setEmiterVolume(1,1,1);
	quazar_jet_top.mPosition.x = 0;
	quazar_jet_top.mPosition.z = 10;
	quazar_jet_top.mSize = 0.3;
	quazar_jet_top.addAffector(&top);
	quazar_jet_top.addAffector(&inverse_gravitiy);
	quazar_jet_top.addAffector(&attraction_point_top);
	
}

void setupTwirl()
{
	twirl1.setMaxParticles(600);
	twirl1.setParticleDrawType(April::PDT_Additive);
	twirl1.setTexture(red);
	twirl1.setEmitterType(April::ET_Point);
	twirl1.mPosition.x = 10;
	twirl1.mPosition.z = -10;
	twirl1.mRandomStartSize = true;
	twirl1.mMinSize = 0.1;
	twirl1.mMaxSize = 0.4;
	twirl1.addAffector(&force1);
	twirl1.addAffector(&force2);
	twirl1.addAffector(&wind);
	twirl1.addAffector(&grav);
	
	twirl2.setMaxParticles(600);
	twirl2.setParticleDrawType(April::PDT_Additive);
	twirl2.setTexture(plasmaball);
	twirl2.setEmitterType(April::ET_Point);
	twirl2.mPosition.x = 10;
	twirl2.mPosition.z = -10;
	twirl2.mRandomStartSize = true;
	twirl2.mMinSize = 0.1;
	twirl2.mMaxSize = 0.4;
	twirl2.addAffector(&force3);
	twirl2.addAffector(&force4);
	twirl2.addAffector(&wind);
	twirl2.addAffector(&grav);
}

void setupRain()
{
	rain.setMaxParticles(240);
	rain.setParticleDrawType(April::PDT_Additive);
	rain.setTexture(raindrop);
	rain.setEmitterType(April::ET_Box);
	rain.setEmiterVolume(6,0,6);
	rain.mPosition.x = -10;
	rain.mPosition.z = -10;
	rain.mPosition.y = 6;
	rain.mRandomStartSize = true;
	rain.mMinSize = 0.1;
	rain.mMaxSize = 0.4;
	rain.addAffector(&gravity_rain);
}

void setupFlame()
{
	flame.setMaxParticles(128);
	flame.setParticleDrawType(April::PDT_Additive);
	flame.setTexture(fire);
	flame.setEmitterType(April::ET_HollowSphere);
	flame.setEmiterVolume(2,4,2);
	flame.mPosition.x = -10;
	flame.mPosition.y = 0;
	flame.mRandomStartAngle = true;
	flame.mRandomStartSize = true;
	flame.mMinSize = 2.4;
	flame.mMaxSize = 3.4;
	flame.addAffector(&colorAffector1);
	flame.addAffector(&linearforce1);
	flame.addAffector(&rotator);
}

void setupBubbles()
{
	bubbles.setMaxParticles(300);
	bubbles.setParticleDrawType(April::PDT_Normal);
	bubbles.setTexture(boubbles);
	bubbles.setEmitterType(April::ET_Cylinder);
	bubbles.setEmiterVolume(5,4,5);
	bubbles.mPosition.x = 0;
	bubbles.mPosition.y = 0;
	bubbles.mRandomStartSize = true;
	bubbles.mMinSize = 0.4;
	bubbles.mMaxSize = 0.8;
	bubbles.addAffector(&colorAffector2);
	bubbles.addAffector(&directionalForce);
}

void setupVortex()
{
	vortex.setMaxParticles(800);
	vortex.setParticleDrawType(April::PDT_Additive);
	vortex.setTexture(red);
	vortex.setEmitterType(April::ET_Sphere);
	vortex.setEmiterVolume(8,0,8);
	vortex.mPosition.x = 10;
	vortex.mPosition.y = 0;
	vortex.mSize = 0.4;
	vortex.addAffector(&swirl);
	vortex.addAffector(&attractor1);
}
int main()
{
	April::init("April",800,600,0,"AprilParticle Demo");
	April::rendersys->registerUpdateCallback(render);
	setupGrid(2.0);
	
	red = April::rendersys->loadTexture("../media/red_particle.png");
	boubbles = April::rendersys->loadTexture("../media/bubbles.png");
	plasmaball = April::rendersys->loadTexture("../media/plasma_ball.png");
	fire = April::rendersys->loadTexture("../media/partikl_strange.png");
	raindrop = April::rendersys->loadTexture("../media/raindrop.png");
	jet = April::rendersys->loadTexture("../media/quazar_jet.png");
	
	setupFlame();
	setupBubbles();
	setupVortex();
	setupQuazar();
	setupRain();
	setupTwirl();
	
	std::map<float, unsigned int> cols1;
	cols1[0.0]  = 0x00FF0000;
	cols1[0.1] = 0xF3FF6432;
	cols1[0.65] = 0xAF3F3F3F;
	cols1[1.0]  = 0x00000000;
	
	colorAffector1.setColors(cols1);
	
	std::map<float, unsigned int> cols2;
	cols2[0.0]  = 0x00FFFFFF;
	cols2[0.04] = 0xFFFFFFF;
	cols2[0.96] = 0xFFFFFFF;
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
