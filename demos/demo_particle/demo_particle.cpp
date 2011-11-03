#include <math.h>

#include <april/main.h>
#include <april/RenderSystem.h>
#include <april/Timer.h>
#include <april/Window.h>
#include <aprilparticle/ParticleEmitter.h>
#include <aprilparticle/aprilparticle.h>

#include <gtypes/Matrix3.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

april::Texture* jet;
april::Texture* plasmaball;
april::Texture* fire;
april::Texture* red;
april::Texture* raindrop;
april::Texture* bubble;

aprilparticle::ParticleEmitter* flame = new aprilparticle::ParticleEmitter(4.0f, 32.0f);
aprilparticle::ParticleEmitter* bubbles = new aprilparticle::ParticleEmitter(6.0f, 50.0f);
aprilparticle::ParticleEmitter* vortex = new aprilparticle::ParticleEmitter(2.0f, 200.0f);

aprilparticle::DirectionalForceField* directionalForce = new aprilparticle::DirectionalForceField(gvec3(0.0f, 1.0f, 0.0f), gvec3(0.0f, 1.0f, 0.0f), 4.0f);
aprilparticle::ColorAffector* colorFader = new aprilparticle::ColorAffector(april::Color(0x1131FFFF), april::Color(0x22000000));
aprilparticle::MultiColorAffector* colorAffector1 = new aprilparticle::MultiColorAffector();
aprilparticle::MultiColorAffector* colorAffector2 = new aprilparticle::MultiColorAffector();
aprilparticle::LinearDirectionalForce* linearforce1 = new aprilparticle::LinearDirectionalForce(gvec3(0.0f, 1.0f, 0.0f), 0.018f);
aprilparticle::Rotator* rotator = new aprilparticle::Rotator(50.0f);
aprilparticle::Attractor* attractor1 = new aprilparticle::Attractor(gvec3(10.0f, 0.0f, 0.0f), 2.0f);
aprilparticle::Swirl* swirl = new aprilparticle::Swirl(gvec3(10.0f, 0.0f, 0.0f), gvec3(0.0f, 1.0f, 0.0f), 8.0f, 0.2f);

// kvazar
aprilparticle::ParticleEmitter* quazar_disc = new aprilparticle::ParticleEmitter(4.0f, 240.0f);
aprilparticle::ParticleEmitter* quazar_jet_top = new aprilparticle::ParticleEmitter(4.0f, 120.0f);
aprilparticle::ParticleEmitter* quazar_jet_bottom = new aprilparticle::ParticleEmitter(4.0f, 120.0f);
aprilparticle::LinearDirectionalForce* top = new aprilparticle::LinearDirectionalForce(gvec3(1.0f, 0.0f, 0.0f), 0.024f);
aprilparticle::LinearDirectionalForce* bottom = new aprilparticle::LinearDirectionalForce(gvec3(-1.0f, 0.0f, 0.0f), 0.024f);
aprilparticle::Attractor* gravity = new aprilparticle::Attractor(gvec3(0.0f, 0.0f, 10.0f), 2.0f);
aprilparticle::Swirl* spin = new aprilparticle::Swirl(gvec3(0.0f, 0.0f, 10.0f), gvec3(1.0f, 0.0f, 0.0f), 8.0f, 0.1f);
aprilparticle::Attractor* inverse_gravitiy = new aprilparticle::Attractor(gvec3(0.0f, 0.0f, 10.0f), -4.0f);
aprilparticle::Attractor* attraction_point_bottom = new aprilparticle::Attractor(gvec3(-10.0f, 0.0f, 10.0f), 6.0f);
aprilparticle::Attractor* attraction_point_top = new aprilparticle::Attractor(gvec3(10.0f, 0.0f, 10.0f), 6.0f);

// kisa
aprilparticle::ParticleEmitter* rain = new aprilparticle::ParticleEmitter(4.0f);
aprilparticle::LinearDirectionalForce* gravity_rain = new aprilparticle::LinearDirectionalForce(gvec3(0.0f, 1.0f, 0.0f), -0.012f);

// twirl
aprilparticle::ParticleEmitter* twirl1 = new aprilparticle::ParticleEmitter(6.0f, 100.0f);
aprilparticle::DirectionalForceField* force1 = new aprilparticle::DirectionalForceField(gvec3(10.0f, 0.0f, -10.0f), gvec3(-1.0f, 1.0f, 0.0f), 4.0f);
aprilparticle::DirectionalForceField* force2 = new aprilparticle::DirectionalForceField(gvec3(10.0f, 4.0f, -10.0f), gvec3(1.0f, 1.0f, 0.0f), 4.0f);

aprilparticle::ParticleEmitter* twirl2 = new aprilparticle::ParticleEmitter(6.0f, 100.0f);
aprilparticle::DirectionalForceField* force3 = new aprilparticle::DirectionalForceField(gvec3(10.0f, 0.0f, -10.0f), gvec3(-1.0f, 1.0f, 0.0f), 4.0f);
aprilparticle::DirectionalForceField* force4 = new aprilparticle::DirectionalForceField(gvec3(10.0f, 4.0f, -10.0f), gvec3(1.0f, 1.0f, 0.0f), 4.0f);

aprilparticle::LinearDirectionalForce* wind = new aprilparticle::LinearDirectionalForce(gvec3(-1.0f, 0.0f, 0.0f), 0.012f);
aprilparticle::Attractor* grav = new aprilparticle::Attractor(gvec3(0.0f, 0.0f, -10.0f), 7.0f);

april::ColoredVertex grid[44];

void setupGrid(float spacing)
{
	int i = 0;
	for (float s = -5 * spacing; s <= 5 * spacing; i++, s += spacing)
	{
		grid[i * 4 + 0].set(-5 * spacing, 0.0f, s);
		grid[i * 4 + 0].color = 0x777777FF;
		grid[i * 4 + 1].set(5 * spacing, 0.0f, s);
		grid[i * 4 + 1].color = 0x777777FF;
		grid[i * 4 + 2].set(s, 0.0f, -5 * spacing);
		grid[i * 4 + 2].color = 0x777777FF;
		grid[i * 4 + 3].set(s, 0.0f, 5 * spacing);
		grid[i * 4 + 3].color = 0x777777FF;
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
	
	flame->update(k);
	flame->draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	
	bubbles->update(k);
	bubbles->draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	
	vortex->update(k);
	vortex->draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	
	// kvazar
	quazar_disc->update(k);
	quazar_jet_bottom->update(k);
	quazar_jet_top->update(k);
	quazar_disc->draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	quazar_jet_bottom->draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	quazar_jet_top->draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	
	// rain
	rain->update(k);
	rain->draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	
	force1->setDirection(gvec3(sin(angle * 0.067f), 0.0f, cos(angle * 0.033f)));
	force2->setDirection(gvec3(sin(angle * 0.067f), sin(angle * 0.033f), 0.0f));
	force3->setDirection(gvec3(cos(angle * 0.033f), 0.0f, sin(angle * 0.067f)));
	force4->setDirection(gvec3(sin(angle * 0.033f), sin(angle * 0.067f), 0.0f));
	
	twirl1->update(k);
	twirl1->draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	
	twirl2->update(k);
	twirl2->draw(pos, gvec3(0.0f, 1.0f, 0.0f));
	
	return true;
}

void setupQuazar()
{
	gvec3 position;

	quazar_disc->setMaxParticles(960);
	quazar_jet_top->setMaxParticles(480);
	quazar_jet_bottom->setMaxParticles(480);
	
	quazar_disc->setBlendMode(april::ADD);
	quazar_jet_top->setBlendMode(april::ADD);
	quazar_jet_bottom->setBlendMode(april::ADD);
	
	quazar_disc->setTexture(plasmaball);
	quazar_jet_bottom->setTexture(jet);
	quazar_jet_top->setTexture(jet);
	
	quazar_disc->setEmitterType(aprilparticle::ET_HollowSphere);
	quazar_disc->setEmitterVolume(6.0f, 6.0f, 1.0f);
	quazar_disc->setPosition(gvec3(0.0f, 0.0f, 10.0f));
	quazar_disc->setSize(0.3f);
	quazar_disc->addAffector(gravity);
	quazar_disc->addAffector(spin);
	
	quazar_jet_bottom->setEmitterType(aprilparticle::ET_Sphere);
	quazar_jet_bottom->setEmitterVolume(1.0f, 1.0f, 1.0f);
	quazar_jet_bottom->setPosition(gvec3(0.0f, 0.0f, 10.0f));
	quazar_jet_bottom->setSize(0.3f);
	quazar_jet_bottom->addAffector(bottom);
	quazar_jet_bottom->addAffector(inverse_gravitiy);
	quazar_jet_bottom->addAffector(attraction_point_bottom);
	
	quazar_jet_top->setEmitterType(aprilparticle::ET_Sphere);
	quazar_jet_top->setEmitterVolume(1.0f, 1.0f, 1.0f);
	quazar_jet_top->setPosition(gvec3(0.0f, 0.0f, 10.0f));
	quazar_jet_top->setSize(0.3f);
	quazar_jet_top->addAffector(top);
	quazar_jet_top->addAffector(inverse_gravitiy);
	quazar_jet_top->addAffector(attraction_point_top);
	
}

void setupTwirl()
{
	twirl1->setMaxParticles(600);
	twirl1->setBlendMode(april::ADD);
	twirl1->setTexture(red);
	twirl1->setEmitterType(aprilparticle::ET_Point);
	twirl1->setPosition(gvec3(10.0f, 0.0f, -10.0f));
	twirl1->setRandomStartSize(true);
	twirl1->setSizeRange(0.1f, 0.4f);
	twirl1->addAffector(force1);
	twirl1->addAffector(force2);
	twirl1->addAffector(wind);
	twirl1->addAffector(grav);
	
	twirl2->setMaxParticles(600);
	twirl2->setBlendMode(april::ADD);
	twirl2->setTexture(plasmaball);
	twirl2->setEmitterType(aprilparticle::ET_Point);
	twirl2->setPosition(gvec3(10.0f, 0.0f, -10.0f));
	twirl2->setRandomStartSize(true);
	twirl2->setSizeRange(0.1f, 0.4f);
	twirl2->addAffector(force3);
	twirl2->addAffector(force4);
	twirl2->addAffector(wind);
	twirl2->addAffector(grav);
}

void setupRain()
{
	rain->setMaxParticles(240);
	rain->setBlendMode(april::ADD);
	rain->setTexture(raindrop);
	rain->setEmitterType(aprilparticle::ET_Box);
	rain->setEmitterVolume(6.0f, 0.0f, 6.0f);
	twirl1->setPosition(gvec3(10.0f, 0.0f, -10.0f));
	rain->setPosition(gvec3(-10.0f, -10.0f, 6.0f));
	rain->setRandomStartSize(true);
	rain->setSizeRange(0.1f, 0.4f);
	rain->addAffector(gravity_rain);
}

void setupFlame()
{
	flame->setMaxParticles(128);
	flame->setBlendMode(april::ADD);
	flame->setTexture(fire);
	flame->setEmitterType(aprilparticle::ET_HollowSphere);
	flame->setEmitterVolume(2.0f, 4.0f, 2.0f);
	flame->setPosition(gvec3(-10.0f, 0.0f, 0.0f));
	flame->setRandomStartAngle(true);
	flame->setRandomStartSize(true);
	flame->setSizeRange(2.4f, 3.4f);
	hmap<float, april::Color> colors1;
	colors1[0.0f] = april::Color(0xFF000000);
	colors1[0.1f] = april::Color(0xFF6432F3);
	colors1[0.4f] = april::Color(0xFF7F3FAF);
	colors1[0.75f] = april::Color(0x7F7F7FAF);
	colors1[1.0f] = april::Color(0x7F7F7F00);
	colorAffector1->setTimeColors(colors1);
	flame->addAffector(colorAffector1);
	flame->addAffector(linearforce1);
	flame->addAffector(rotator);
}

void setupBubbles()
{
	bubbles->setMaxParticles(300);
	bubbles->setBlendMode(april::DEFAULT);
	bubbles->setTexture(bubble);
	bubbles->setEmitterType(aprilparticle::ET_Cylinder);
	bubbles->setEmitterVolume(5.0f, 4.0f, 5.0f);
	bubbles->setPosition(gvec3(0.0f, 0.0f, 0.0f));
	bubbles->setRandomStartSize(true);
	bubbles->setSizeRange(0.4f, 0.8f);
	hmap<float, april::Color> colors2;
	colors2[0.0f] = april::Color(0xFFFFFF00);
	colors2[0.04f] = april::Color(0xFFFFFFFF);
	colors2[0.99f] = april::Color(0xFFFFFFFF);
	colors2[1.0f] = april::Color(0xFFFFFF00);
	colorAffector2->setTimeColors(colors2);
	bubbles->addAffector(colorAffector2);
	bubbles->addAffector(directionalForce);
}

void setupVortex()
{
	vortex->setMaxParticles(800);
	vortex->setBlendMode(april::ADD);
	vortex->setTexture(red);
	vortex->setEmitterType(aprilparticle::ET_Sphere);
	vortex->setEmitterVolume(8.0f, 0.0f, 8.0f);
	vortex->setPosition(gvec3(10.0f, 0.0f, 0.0f));
	vortex->setSize(0.4f);
	vortex->addAffector(swirl);
	vortex->addAffector(attractor1);
}

void april_init(const harray<hstr>& args)
{
#ifdef __APPLE__
	// On MacOSX, the current working directory is not set by
	// the Finder, since you are expected to use Core Foundation
	// or ObjC APIs to find files. 
	// So, when porting you probably want to set the current working
	// directory to something sane (e.g. .../Resources/ in the app
	// bundle).
	// In this case, we set it to parent of the .app bundle.
	{	// curly braces in order to localize variables 

		CFURLRef url = CFBundleCopyBundleURL(CFBundleGetMainBundle());
		CFStringRef path = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
		
		// let's hope chdir() will be happy with utf8 encoding
		const char* cpath = CFStringGetCStringPtr(path, kCFStringEncodingUTF8);
		char* cpath_alloc = 0;
		if (!cpath)
		{
			// CFStringGetCStringPtr is allowed to return NULL. bummer.
			// we need to use CFStringGetCString instead.
			cpath_alloc = (char*)malloc(CFStringGetLength(path) + 1);
			CFStringGetCString(path, cpath_alloc, CFStringGetLength(path) + 1, kCFStringEncodingUTF8);
		}
		else
		{
			// even though it didn't return NULL, we still want to slice off bundle name.
			cpath_alloc = (char*)malloc(CFStringGetLength(path) + 1);
			strcpy(cpath_alloc, cpath);
		}
		// just in case / is appended to .app path for some reason
		if (cpath_alloc[CFStringGetLength(path) - 1] == '/')
		{
			cpath_alloc[CFStringGetLength(path) - 1] = 0;
		}
		
		// replace pre-.app / with a null character, thus
		// cutting off .app's name and getting parent of .app.
		strrchr(cpath_alloc, '/')[0] = 0;
							   
		// change current dir using posix api
		chdir(cpath_alloc);
		
		free(cpath_alloc); // even if null, still ok
		CFRelease(path);
		CFRelease(url);
	}
#endif
	april::init();
	april::createRenderSystem("");
	april::createRenderTarget(1024, 768, false, "AprilParticle Demo");
	aprilparticle::init();
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
	
	//emitter->addAffector(&force1);
	//emitter->addAffector(&colorFader);
	
	//emitter->addAffector(&attractor1);
	//emitter->addAffector(&swirl);
    
}

void april_destroy()
{
	aprilparticle::destroy();
	april::destroy();
}

