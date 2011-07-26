#include <math.h>

#include <april/main.h>
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

april::ParticleEmitter flame(4, 32);
april::ParticleEmitter bubbles(6, 50);
april::ParticleEmitter vortex(2, 200);

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
april::Texture* bubble;

// kvazar
april::ParticleEmitter quazar_disc(4, 240);
april::ParticleEmitter quazar_jet_top(4, 120);
april::ParticleEmitter quazar_jet_bottom(4, 120);
april::Affectors::LinearDirectionalForce top(gvec3(1.0f, 0.0f, 0.0f), 24), bottom(gvec3(-1.0f, 0.0f, 0.0f), 24);
april::Affectors::Attractor gravity(gvec3(0.0f, 0.0f, 10.0f), 2.0f);
april::Affectors::Swirl spin(gvec3(0.0f, 0.0f, 10.0f), gvec3(1.0f, 0.0f, 0.0f), 8.0f, 0.1f);
april::Affectors::Attractor inverse_gravitiy(gvec3(0.0f, 0.0f, 10.0f), -4.0f);
april::Affectors::Attractor attraction_point_bottom(gvec3(-10.0f, 0.0f, 10.0f), 6.0f);
april::Affectors::Attractor attraction_point_top(gvec3(10.0f, 0.0f, 10.0f), 6.0f);

// kisa
april::ParticleEmitter rain(4, 60);
april::Affectors::LinearDirectionalForce gravity_rain(gvec3(0.0f, 1.0f, 0.0f), -12);

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

bool render(float k)
{
	april::rendersys->clear(true, false);
	static float angle = 0.0f;
	angle += k * 90.0f;
    
	april::rendersys->setPerspective(60.0f, 1024.0f / 768.0f, 0.1f, 100.0f);
	
	gvec3 pos(0.0f, 18.0f, 25.0f);
	gmat3 rot;
	rot.setRotation3D(0.0f, 1.0f, 0.0f, angle * 0.2f);
	pos = rot * pos;
	april::rendersys->lookAt(pos, gvec3(0.0f, 0.0f, 0.0f), gvec3(0.0f, 1.0f, 0.0f));
	drawGrid();
	
	flame.update(k);
	flame.draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	
	bubbles.update(k);
	bubbles.draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	
	vortex.update(k);
	vortex.draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	
	// kvazar
	quazar_disc.update(k);
	quazar_jet_bottom.update(k);
	quazar_jet_top.update(k);
	quazar_disc.draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	quazar_jet_bottom.draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	quazar_jet_top.draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	
	// rain
	rain.update(k);
	rain.draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	
	gvec3 direction;
	direction = force1.getDirection();
	direction.x = sin(angle * 0.067f);
	direction.z = cos(angle * 0.033f);
	force1.setDirection(direction);
	
	direction = force2.getDirection();
	direction.y = sin(angle * 0.033f);
	direction.x = sin(angle * 0.067f);
	force2.setDirection(direction);
	
	direction = force3.getDirection();
	direction.z = sin(angle * 0.067f);
	direction.x = cos(angle * 0.033f);
	force3.setDirection(direction);
	
	direction = force4.getDirection();
	direction.x = sin(angle * 0.033f);
	direction.y = sin(angle * 0.067f);
	force4.setDirection(direction);
	
	twirl1.update(k);
	twirl1.draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	
	twirl2.update(k);
	twirl2.draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	
	return true;
}

void setupQuazar()
{
	gvec3 position;

	quazar_disc.setMaxParticles(960);
	quazar_jet_top.setMaxParticles(480);
	quazar_jet_bottom.setMaxParticles(480);
	
	quazar_disc.setBlendMode(april::ADD);
	quazar_jet_top.setBlendMode(april::ADD);
	quazar_jet_bottom.setBlendMode(april::ADD);
	
	quazar_disc.setTexture(plasmaball);
	quazar_jet_bottom.setTexture(jet);
	quazar_jet_top.setTexture(jet);
	
	quazar_disc.setEmitterType(april::ET_HollowSphere);
	quazar_disc.setEmitterVolume(6.0f, 6.0f, 1.0f);
	quazar_disc.setPosition(gvec3(0.0f, 0.0f, 10.0f));
	quazar_disc.setSize(0.3f);
	quazar_disc.addAffector(&gravity);
	quazar_disc.addAffector(&spin);
	
	quazar_jet_bottom.setEmitterType(april::ET_Sphere);
	quazar_jet_bottom.setEmitterVolume(1.0f, 1.0f, 1.0f);
	quazar_jet_bottom.setPosition(gvec3(0.0f, 0.0f, 10.0f));
	quazar_jet_bottom.setSize(0.3f);
	quazar_jet_bottom.addAffector(&bottom);
	quazar_jet_bottom.addAffector(&inverse_gravitiy);
	quazar_jet_bottom.addAffector(&attraction_point_bottom);
	
	quazar_jet_top.setEmitterType(april::ET_Sphere);
	quazar_jet_top.setEmitterVolume(1.0f, 1.0f, 1.0f);
	quazar_jet_top.setPosition(gvec3(0.0f, 0.0f, 10.0f));
	quazar_jet_top.setSize(0.3f);
	quazar_jet_top.addAffector(&top);
	quazar_jet_top.addAffector(&inverse_gravitiy);
	quazar_jet_top.addAffector(&attraction_point_top);
	
}

void setupTwirl()
{
	twirl1.setMaxParticles(600);
	twirl1.setBlendMode(april::ADD);
	twirl1.setTexture(red);
	twirl1.setEmitterType(april::ET_Point);
	twirl1.setPosition(gvec3(10.0f, 0.0f, -10.0f));
	twirl1.setRandomStartSize(true);
	twirl1.setSizeRange(0.1f, 0.4f);
	twirl1.addAffector(&force1);
	twirl1.addAffector(&force2);
	twirl1.addAffector(&wind);
	twirl1.addAffector(&grav);
	
	twirl2.setMaxParticles(600);
	twirl2.setBlendMode(april::ADD);
	twirl2.setTexture(plasmaball);
	twirl2.setEmitterType(april::ET_Point);
	twirl2.setPosition(gvec3(10.0f, 0.0f, -10.0f));
	twirl2.setRandomStartSize(true);
	twirl2.setSizeRange(0.1f, 0.4f);
	twirl2.addAffector(&force3);
	twirl2.addAffector(&force4);
	twirl2.addAffector(&wind);
	twirl2.addAffector(&grav);
}

void setupRain()
{
	rain.setMaxParticles(240);
	rain.setBlendMode(april::ADD);
	rain.setTexture(raindrop);
	rain.setEmitterType(april::ET_Box);
	rain.setEmitterVolume(6.0f, 0.0f, 6.0f);
	twirl1.setPosition(gvec3(10.0f, 0.0f, -10.0f));
	rain.setPosition(gvec3(-10.0f, -10.0f, 6.0f));
	rain.setRandomStartSize(true);
	rain.setSizeRange(0.1f, 0.4f);
	rain.addAffector(&gravity_rain);
}

void setupFlame()
{
	flame.setMaxParticles(128);
	flame.setBlendMode(april::ADD);
	flame.setTexture(fire);
	flame.setEmitterType(april::ET_HollowSphere);
	flame.setEmitterVolume(2.0f, 4.0f, 2.0f);
	flame.setPosition(gvec3(-10.0f, 0.0f, 0.0f));
	flame.setRandomStartAngle(true);
	flame.setRandomStartSize(true);
	flame.setSizeRange(2.4f, 3.4f);
	flame.addAffector(&colorAffector1);
	flame.addAffector(&linearforce1);
	flame.addAffector(&rotator);
}

void setupBubbles()
{
	bubbles.setMaxParticles(300);
	bubbles.setBlendMode(april::DEFAULT);
	bubbles.setTexture(bubble);
	bubbles.setEmitterType(april::ET_Cylinder);
	bubbles.setEmitterVolume(5.0f, 4.0f, 5.0f);
	bubbles.setPosition(gvec3(0.0f, 0.0f, 0.0f));
	bubbles.setRandomStartSize(true);
	bubbles.setSizeRange(0.4f, 0.8f);
	bubbles.addAffector(&colorAffector2);
	bubbles.addAffector(&directionalForce);
}

void setupVortex()
{
	vortex.setMaxParticles(800);
	vortex.setBlendMode(april::ADD);
	vortex.setTexture(red);
	vortex.setEmitterType(april::ET_Sphere);
	vortex.setEmitterVolume(8.0f, 0.0f, 8.0f);
	vortex.setPosition(gvec3(10.0f, 0.0f, 0.0f));
	vortex.setSize(0.4f);
	vortex.addAffector(&swirl);
	vortex.addAffector(&attractor1);
}

void april_init(const harray<hstr>& args)
{
	april::init();
	april::createRenderSystem("");
	april::createRenderTarget(800, 600, false, "AprilParticle Demo");
	april::rendersys->getWindow()->setUpdateCallback(render);
	setupGrid(2.0f);
	
	red = april::rendersys->loadTexture("../media/red_particle.png");
	bubble = april::rendersys->loadTexture("../media/bubbles.png");
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
	
	hmap<float, unsigned int> colors1;
	colors1[0.0f]  = 0xFF000000;
	colors1[0.1f] = 0xFF6432F3;
	colors1[0.65f] = 0x3F3F3FAF;
	colors1[1.0f]  = 0x00000000;
	
	colorAffector1.setColors(colors1);
	
	hmap<float, unsigned int> colors2;
	colors2[0.0f]  = 0xFFFFFF00;
	colors2[0.04f] = 0xFFFFFFFF;
	colors2[0.96f] = 0xFFFFFFFF;
	colors2[1.0f]  = 0xFFFFFF00;
	
	colorAffector2.setColors(colors2);
	
	
	//emitter->addAffector(&force1);
	//emitter->addAffector(&colorFader);
	
	//emitter->addAffector(&attractor1);
	//emitter->addAffector(&swirl);
    
}

void april_destroy()
{
	april::destroy();
}
