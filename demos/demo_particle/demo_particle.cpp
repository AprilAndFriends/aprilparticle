#include <math.h>

#include <april/RenderSystem.h>
#include <april/Timer.h>
#include <april/Window.h>
#include <aprilparticle/ParticleEmitter.h>
//#include <aprilutil/StaticMesh.h>

#include <gtypes/Matrix3.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

april::ParticleEmitter flame(4, 32), bubbles(6, 50), vortex(2, 200);

april::Affectors::DirectionalForceField directionalForce(4.0f, gvec3(0.0f, 1.0f, 0.0f), gvec3(0.0f, 1.0f, 0.0f));
april::Affectors::ColorAffector colorFader(0x1131FFFF, 0x22000000);
april::Affectors::MultiColorAffector colorAffector1, colorAffector2;
april::Affectors::LinearDirectionalForce linearforce1(gvec3(0.0f, 1.0f, 0.0f), 18.0f);
april::Affectors::Rotator rotator(50.0f);
april::Affectors::Attractor attractor1(gvec3(10.0f, 0.0f, 0.0f), 2.0f);
april::Affectors::Swirl swirl(gvec3(10.0f, 0.0f, 0.0f), gvec3(0.0f, 1.0f, 0.0f), 8.0f, 0.2f);

april::Texture* jet;
april::Texture* plasmaball;
april::Texture* fire;
april::Texture* red;
april::Texture* raindrop;
april::Texture* bubbles;

// kvazar
april::ParticleEmitter quazar_disc(4, 240), quazar_jet_top(4, 120), quazar_jet_bottom(4, 120);
april::Affectors::LinearDirectionalForce top(gvec3(1.0f, 0.0f, 0.0f), 24), bottom(gvec3(-1.0f, 0.0f, 0.0f), 24);
april::Affectors::Attractor gravity(gvec3(0.0f, 0.0f, 10.0f), 2.0f);
april::Affectors::Swirl spin(gvec3(0.0f, 0.0f, 10.0f), gvec3(1.0f, 0.0f, 0.0f), 8.0f, 0.1f);
april::Affectors::Attractor inverse_gravitiy(gvec3(0.0f, 0.0f, 10.0f), -4.0f);
april::Affectors::Attractor attraction_point_bottom(gvec3(-10.0f, 0.0f, 10.0f), 6.0f);
april::Affectors::Attractor attraction_point_top(gvec3(10.0f, 0.0f, 10.0f), 6.0f);

// kisa
april::ParticleEmitter rain(4, 60);
april::Affectors::LinearDirectionalForce gravity_rain(gvec3(0,1,0), -12);

// twirl
april::ParticleEmitter twirl1(6, 100);
april::Affectors::DirectionalForceField force1(4.0, gvec3(10.0f, 0.0f, -10.0f), gvec3(-1.0f, 1.0f, 0.0f));
april::Affectors::DirectionalForceField force2(4.0, gvec3(10.0f, 4.0f, -10.0f), gvec3(1.0f, 1.0f, 0.0f));

april::ParticleEmitter twirl2(6, 100);
april::Affectors::DirectionalForceField force3(4.0, gvec3(10.0f, 0.0f, -10.0f), gvec3(-1.0f, 1.0f, 0.0f));
april::Affectors::DirectionalForceField force4(4.0, gvec3(10.0f, 4.0f, -10.0f), gvec3(1.0f, 1.0f, 0.0f));

april::Affectors::LinearDirectionalForce wind(gvec3(-1.0f, 0.0f, 0.0f), 12.0f);
april::Affectors::Attractor grav(gvec3(0.0f, 0.0f, -10.0f), 7.0f);


april::ColoredVertex grid[44];

april::ColoredVertex u, v, p, r;

void setupGrid(float spacing)
{
	int i = 0;
	for (float s = -5 * spacing; s <= 5 * spacing; i++, s += spacing)
	{
		u.color = v.color = p.color = r.color = 0x222222FF;
		u.x = -5 * spacing;
		u.y = 0.0f;
		u.z = s;
		v.x = 5 * spacing;
		v.y = 0.0f;
		v.z = s;
		grid[i * 4 + 0] = u;
		grid[i * 4 + 1] = v;
		
		p.x = s;
		p.y = 0.0f;
		p.z = -5 * spacing;
		r.x = s;
		r.y = 0.0f;
		r.z = 5 * spacing;
		grid[i * 4 + 2] = p;
		grid[i * 4 + 3] = r;
	}
}

void drawGrid()
{
	april::rendersys->render(april::LineList, grid, 44);
}

bool render(float time_increase)
{
	april::rendersys->clear(true, false);
	static float angle=0;
	angle+=time_increase*90;
    
	april::rendersys->setPerspective(60,1024/768.,0.1f,100.0f);
	
	gvec3 pos(0,18,25);
	gmat3 rot;
	rot.setRotation3D(0,1,0, angle * 0.2);
	pos = rot * pos;
	april::rendersys->lookAt(pos, gvec3(0.0f, 0.0f, 0.0f), gvec3(0.0f, 1.0f, 0.0f));
	drawGrid();
	
	flame.update(time_increase);
	flame.draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	
	bubbles.update(time_increase);
	bubbles.draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	
	vortex.update(time_increase);
	vortex.draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	
	// kvazar
	quazar_disc.update(time_increase);
	quazar_jet_bottom.update(time_increase);
	quazar_jet_top.update(time_increase);
	quazar_disc.draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	quazar_jet_bottom.draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	quazar_jet_top.draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	
	// rain
	rain.update(time_increase);
	rain.draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	
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
	
	quazar_disc.setParticleDrawType(april::PDT_Additive);
	quazar_jet_top.setParticleDrawType(april::PDT_Additive);
	quazar_jet_bottom.setParticleDrawType(april::PDT_Additive);
	
	quazar_disc.setTexture(plasmaball);
	quazar_jet_bottom.setTexture(jet);
	quazar_jet_top.setTexture(jet);
	
	quazar_disc.setEmitterType(april::ET_HollowSphere);
	quazar_disc.setEmitterVolume(6,6,1);
	quazar_disc.mPosition.x = 0;
	quazar_disc.mPosition.z = 10;
	quazar_disc.mSize = 0.3;
	quazar_disc.addAffector(&gravity);
	quazar_disc.addAffector(&spin);
	
	quazar_jet_bottom.setEmitterType(april::ET_Sphere);
	quazar_jet_bottom.setEmitterVolume(1,1,1);
	quazar_jet_bottom.mPosition.x = 0;
	quazar_jet_bottom.mPosition.z = 10;
	quazar_jet_bottom.mSize = 0.3;
	quazar_jet_bottom.addAffector(&bottom);
	quazar_jet_bottom.addAffector(&inverse_gravitiy);
	quazar_jet_bottom.addAffector(&attraction_point_bottom);
	
	quazar_jet_top.setEmitterType(april::ET_Sphere);
	quazar_jet_top.setEmitterVolume(1,1,1);
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
	twirl1.setParticleDrawType(april::PDT_Additive);
	twirl1.setTexture(red);
	twirl1.setEmitterType(april::ET_Point);
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
	twirl2.setParticleDrawType(april::PDT_Additive);
	twirl2.setTexture(plasmaball);
	twirl2.setEmitterType(april::ET_Point);
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
	rain.setParticleDrawType(april::PDT_Additive);
	rain.setTexture(raindrop);
	rain.setEmitterType(april::ET_Box);
	rain.setEmitterVolume(6,0,6);
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
	flame.setParticleDrawType(april::PDT_Additive);
	flame.setTexture(fire);
	flame.setEmitterType(april::ET_HollowSphere);
	flame.setEmitterVolume(2,4,2);
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
	bubbles.setParticleDrawType(april::PDT_Normal);
	bubbles.setTexture(bubbles);
	bubbles.setEmitterType(april::ET_Cylinder);
	bubbles.setEmitterVolume(5,4,5);
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
	vortex.setParticleDrawType(april::PDT_Additive);
	vortex.setTexture(red);
	vortex.setEmitterType(april::ET_Sphere);
	vortex.setEmitterVolume(8,0,8);
	vortex.mPosition.x = 10;
	vortex.mPosition.y = 0;
	vortex.mSize = 0.4;
	vortex.addAffector(&swirl);
	vortex.addAffector(&attractor1);
}

void april_init()
{
	april::init();
	april::createRenderSystem("");
	april::createRenderTarget(800, 600, false, "AprilParticle Demo");
	april::rendersys->getWindow()->setUpdateCallback(render);
	setupGrid(2.0f);
	
	red = april::rendersys->loadTexture("../media/red_particle.png");
	bubbles = april::rendersys->loadTexture("../media/bubbles.png");
	plasmaball = april::rendersys->loadTexture("../media/plasma_ball.png");
	fire = april::rendersys->loadTexture("../media/partikl_strange.png");
	raindrop = april::rendersys->loadTexture("../media/raindrop.png");
	jet = april::rendersys->loadTexture("../media/quazar_jet.png");
	
	setupFlame();
	setupBubbles();
	setupVortex();
	setupQuazar();
	setupRain();
	setupTwirl();
	
	hmap<float, unsigned int> cols1;
	cols1[0.0f]  = 0xFF000000;
	cols1[0.1f] = 0xFF6432F3;
	cols1[0.65f] = 0x3F3F3FAF;
	cols1[1.0f]  = 0x00000000;
	
	colorAffector1.setColors(cols1);
	
	hmap<float, unsigned int> cols2;
	cols2[0.0f]  = 0xFFFFFF00;
	cols2[0.04f] = 0xFFFFFFFF;
	cols2[0.96f] = 0xFFFFFFFF;
	cols2[1.0f]  = 0xFFFFFF00;
	
	colorAffector2.setColors(cols2);
	
	
	//emiter->addAffector(&force1);
	//emiter->addAffector(&colorFader);
	
	//emiter->addAffector(&attractor1);
	//emiter->addAffector(&swirl);
    
}

void april_destroy()
{
	april::destroy();
}
