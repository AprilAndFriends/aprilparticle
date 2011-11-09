#include <math.h>

#include <april/main.h>
#include <april/RenderSystem.h>
#include <april/Timer.h>
#include <april/Window.h>
#include <aprilparticle/aprilparticle.h>
#include <aprilparticle/Affectors.h>
#include <aprilparticle/ParticleEmitter.h>

#include <gtypes/Matrix3.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

gvec3 up(0.0f, 1.0f, 0.0f);

april::Texture* jet;
april::Texture* plasmaball;
april::Texture* fire;
april::Texture* red;
april::Texture* raindrop;
april::Texture* bubble;

aprilparticle::ParticleEmitter* flame;
aprilparticle::ParticleEmitter* bubbles;
aprilparticle::ParticleEmitter* vortex;
aprilparticle::ParticleEmitter* rain;
aprilparticle::ParticleEmitter* quazar_disc;
aprilparticle::ParticleEmitter* quazar_jet_top;
aprilparticle::ParticleEmitter* quazar_jet_bottom;
aprilparticle::ParticleEmitter* twirl1;
aprilparticle::ParticleEmitter* twirl2;

aprilparticle::Affectors::DirectionalForceField* force1;
aprilparticle::Affectors::DirectionalForceField* force2;
aprilparticle::Affectors::DirectionalForceField* force3;
aprilparticle::Affectors::DirectionalForceField* force4;

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
	bubbles->update(k);
	vortex->update(k);
	quazar_disc->update(k);
	quazar_jet_bottom->update(k);
	quazar_jet_top->update(k);
	rain->update(k);
	twirl1->update(k);
	twirl2->update(k);

	force1->setDirection(gvec3(sin(angle * 0.06666667f), 0.0f, cos(angle * 0.03333333f)));
	force2->setDirection(gvec3(sin(angle * 0.06666667f), sin(angle * 0.03333333f), 0.0f));
	force3->setDirection(gvec3(cos(angle * 0.03333333f), 0.0f, sin(angle * 0.06666667f)));
	force4->setDirection(gvec3(sin(angle * 0.03333333f), sin(angle * 0.06666667f), 0.0f));
	
	flame->draw(pos, up);
	bubbles->draw(pos, up);
	vortex->draw(pos, up);
	quazar_disc->draw(pos, up);
	quazar_jet_bottom->draw(pos, up);
	quazar_jet_top->draw(pos, up);
	rain->draw(pos, up);
	twirl1->draw(pos, up);
	twirl2->draw(pos, up);
	
	return true;
}

void setupFlame()
{
	aprilparticle::Affectors::MultiColorChanger* colorChanger = new aprilparticle::Affectors::MultiColorChanger();
	aprilparticle::Affectors::LinearDirectionalForce* linearforce = new aprilparticle::Affectors::LinearDirectionalForce(gvec3(0.0f, 1.0f, 0.0f), 0.018f);
	aprilparticle::Affectors::Rotator* rotator = new aprilparticle::Affectors::Rotator(50.0f);
	flame = new aprilparticle::ParticleEmitter();

	flame->setLife(4.0f);
	flame->setEmissionRate(32.0f);
	flame->setParticleLimit(128);
	flame->setBlendMode(april::ADD);
	flame->setTexture(fire);
	flame->setType(aprilparticle::ParticleEmitter::HollowSphere);
	flame->setDimensions(2.0f, 4.0f, 2.0f);
	flame->setPosition(-10.0f, 0.0f, 0.0f);
	flame->setScaleRange(2.4f, 3.4f);
	flame->setAngleRange(0.0f, 360.0f);
	hmap<float, april::Color> colors;
	colors[0.0f] = april::Color(0xFF000000);
	colors[0.1f] = april::Color(0xFF6432F3);
	colors[0.4f] = april::Color(0xFF7F3FAF);
	colors[0.75f] = april::Color(0x7F7F7FAF);
	colors[1.0f] = april::Color(0x7F7F7F00);
	colorChanger->setColorTimings(colors);
	flame->registerAffector(colorChanger);
	flame->registerAffector(linearforce);
	flame->registerAffector(rotator);
}

void setupBubbles()
{
	aprilparticle::Affectors::MultiColorChanger* colorChanger = new aprilparticle::Affectors::MultiColorChanger();
	aprilparticle::Affectors::DirectionalForceField* directionalForce = new aprilparticle::Affectors::DirectionalForceField(gvec3(0.0f, 1.0f, 0.0f), gvec3(0.0f, 1.0f, 0.0f), 4.0f);
	bubbles = new aprilparticle::ParticleEmitter();

	bubbles->setLife(6.0f);
	bubbles->setEmissionRate(50.0f);
	bubbles->setParticleLimit(300);
	bubbles->setBlendMode(april::DEFAULT);
	bubbles->setTexture(bubble);
	bubbles->setType(aprilparticle::ParticleEmitter::Cylinder);
	bubbles->setDimensions(5.0f, 4.0f, 5.0f);
	bubbles->setPosition(0.0f, 0.0f, 0.0f);
	bubbles->setScaleRange(0.4f, 0.8f);
	hmap<float, april::Color> colors;
	colors[0.0f] = april::Color(0xFFFFFF00);
	colors[0.04f] = april::Color(0xFFFFFFFF);
	colors[0.99f] = april::Color(0xFFFFFFFF);
	colors[1.0f] = april::Color(0xFFFFFF00);
	colorChanger->setColorTimings(colors);
	bubbles->registerAffector(colorChanger);
	bubbles->registerAffector(directionalForce);
}

void setupVortex()
{
	aprilparticle::Affectors::Attractor* attractor1 = new aprilparticle::Affectors::Attractor(gvec3(10.0f, 0.0f, 0.0f), 2.0f);
	aprilparticle::Affectors::Swirl* swirl = new aprilparticle::Affectors::Swirl(gvec3(10.0f, 0.0f, 0.0f), gvec3(0.0f, 1.0f, 0.0f), 0.2f, 8.0f);
	vortex = new aprilparticle::ParticleEmitter();

	vortex->setLife(2.0f);
	vortex->setEmissionRate(200.0f);
	vortex->setParticleLimit(800);
	vortex->setBlendMode(april::ADD);
	vortex->setTexture(red);
	vortex->setType(aprilparticle::ParticleEmitter::Sphere);
	vortex->setDimensions(8.0f, 0.0f, 8.0f);
	vortex->setPosition(10.0f, 0.0f, 0.0f);
	vortex->setScale(0.4f);
	vortex->registerAffector(swirl);
	vortex->registerAffector(attractor1);
}

void setupRain()
{
	aprilparticle::Affectors::LinearDirectionalForce* gravity = new aprilparticle::Affectors::LinearDirectionalForce(gvec3(0.0f, 1.0f, 0.0f), -0.012f);
	rain = new aprilparticle::ParticleEmitter();

	rain->setLife(4.0f);
	rain->setEmissionRate(60.0f);
	rain->setParticleLimit(240);
	rain->setBlendMode(april::ADD);
	rain->setTexture(raindrop);
	rain->setType(aprilparticle::ParticleEmitter::Box);
	rain->setDimensions(6.0f, 0.0f, 6.0f);
	rain->setPosition(-10.0f, -10.0f, 6.0f);
	rain->setScaleRange(0.1f, 0.4f);
	rain->registerAffector(gravity);
}

void setupQuazar()
{
	aprilparticle::Affectors::LinearDirectionalForce* top = new aprilparticle::Affectors::LinearDirectionalForce(gvec3(1.0f, 0.0f, 0.0f), 0.024f);
	aprilparticle::Affectors::LinearDirectionalForce* bottom = new aprilparticle::Affectors::LinearDirectionalForce(gvec3(-1.0f, 0.0f, 0.0f), 0.024f);
	aprilparticle::Affectors::Attractor* gravity = new aprilparticle::Affectors::Attractor(gvec3(0.0f, 0.0f, 10.0f), 2.0f);
	aprilparticle::Affectors::Swirl* spin = new aprilparticle::Affectors::Swirl(gvec3(0.0f, 0.0f, 10.0f), gvec3(1.0f, 0.0f, 0.0f), 0.1f, 8.0f);
	aprilparticle::Affectors::Attractor* inverseGravity1 = new aprilparticle::Affectors::Attractor(gvec3(0.0f, 0.0f, 10.0f), -4.0f);
	aprilparticle::Affectors::Attractor* inverseGravity2 = new aprilparticle::Affectors::Attractor(gvec3(0.0f, 0.0f, 10.0f), -4.0f);
	aprilparticle::Affectors::Attractor* attractionPointBottom = new aprilparticle::Affectors::Attractor(gvec3(-10.0f, 0.0f, 10.0f), 6.0f);
	aprilparticle::Affectors::Attractor* attractionPointTop = new aprilparticle::Affectors::Attractor(gvec3(10.0f, 0.0f, 10.0f), 6.0f);
	quazar_disc = new aprilparticle::ParticleEmitter();
	quazar_jet_top = new aprilparticle::ParticleEmitter();
	quazar_jet_bottom = new aprilparticle::ParticleEmitter();

	quazar_disc->setLife(4.0f);
	quazar_jet_top->setLife(4.0f);
	quazar_jet_bottom->setLife(4.0f);
	
	quazar_disc->setEmissionRate(240.0f);
	quazar_jet_top->setEmissionRate(120.0f);
	quazar_jet_bottom->setEmissionRate(120.0f);
	
	quazar_disc->setParticleLimit(960);
	quazar_jet_top->setParticleLimit(480);
	quazar_jet_bottom->setParticleLimit(480);
	
	quazar_disc->setBlendMode(april::ADD);
	quazar_jet_top->setBlendMode(april::ADD);
	quazar_jet_bottom->setBlendMode(april::ADD);
	
	quazar_disc->setTexture(plasmaball);
	quazar_jet_top->setTexture(jet);
	quazar_jet_bottom->setTexture(jet);
	
	quazar_disc->setType(aprilparticle::ParticleEmitter::HollowSphere);
	quazar_disc->setDimensions(1.0f, 6.0f, 6.0f);
	quazar_disc->setPosition(0.0f, 0.0f, 10.0f);
	quazar_disc->setScale(0.3f);
	quazar_disc->registerAffector(gravity);
	quazar_disc->registerAffector(spin);
	
	quazar_jet_bottom->setType(aprilparticle::ParticleEmitter::Sphere);
	quazar_jet_bottom->setDimensions(1.0f, 1.0f, 1.0f);
	quazar_jet_bottom->setPosition(0.0f, 0.0f, 10.0f);
	quazar_jet_bottom->setScale(0.3f);
	quazar_jet_bottom->registerAffector(bottom);
	quazar_jet_bottom->registerAffector(inverseGravity1);
	quazar_jet_bottom->registerAffector(attractionPointBottom);
	
	quazar_jet_top->setType(aprilparticle::ParticleEmitter::Sphere);
	quazar_jet_top->setDimensions(1.0f, 1.0f, 1.0f);
	quazar_jet_top->setPosition(0.0f, 0.0f, 10.0f);
	quazar_jet_top->setScale(0.3f);
	quazar_jet_top->registerAffector(top);
	quazar_jet_top->registerAffector(inverseGravity2);
	quazar_jet_top->registerAffector(attractionPointTop);
	
}

void setupTwirl()
{
	aprilparticle::Affectors::LinearDirectionalForce* wind1 = new aprilparticle::Affectors::LinearDirectionalForce(gvec3(-1.0f, 0.0f, 0.0f), 0.012f);
	aprilparticle::Affectors::Attractor* gravity1 = new aprilparticle::Affectors::Attractor(gvec3(0.0f, 0.0f, -10.0f), 7.0f);
	aprilparticle::Affectors::LinearDirectionalForce* wind2 = new aprilparticle::Affectors::LinearDirectionalForce(gvec3(-1.0f, 0.0f, 0.0f), 0.012f);
	aprilparticle::Affectors::Attractor* gravity2 = new aprilparticle::Affectors::Attractor(gvec3(0.0f, 0.0f, -10.0f), 7.0f);
	force1 = new aprilparticle::Affectors::DirectionalForceField(gvec3(10.0f, 0.0f, -10.0f), gvec3(-1.0f, 1.0f, 0.0f), 4.0f);
	force2 = new aprilparticle::Affectors::DirectionalForceField(gvec3(10.0f, 4.0f, -10.0f), gvec3(1.0f, 1.0f, 0.0f), 4.0f);
	force3 = new aprilparticle::Affectors::DirectionalForceField(gvec3(10.0f, 0.0f, -10.0f), gvec3(-1.0f, 1.0f, 0.0f), 4.0f);
	force4 = new aprilparticle::Affectors::DirectionalForceField(gvec3(10.0f, 4.0f, -10.0f), gvec3(1.0f, 1.0f, 0.0f), 4.0f);
	twirl1 = new aprilparticle::ParticleEmitter();
	twirl2 = new aprilparticle::ParticleEmitter();

	twirl1->setLife(6.0f);
	twirl1->setEmissionRate(100.0f);
	twirl1->setParticleLimit(600);
	twirl1->setBlendMode(april::ADD);
	twirl1->setTexture(red);
	twirl1->setType(aprilparticle::ParticleEmitter::Point);
	twirl1->setPosition(10.0f, 0.0f, -10.0f);
	twirl1->setScaleRange(0.1f, 0.4f);
	twirl1->registerAffector(force1);
	twirl1->registerAffector(force2);
	twirl1->registerAffector(wind1);
	twirl1->registerAffector(gravity1);
	
	twirl2->setLife(6.0f);
	twirl2->setEmissionRate(100.0f);
	twirl2->setParticleLimit(600);
	twirl2->setBlendMode(april::ADD);
	twirl2->setTexture(plasmaball);
	twirl2->setType(aprilparticle::ParticleEmitter::Point);
	twirl2->setPosition(10.0f, 0.0f, -10.0f);
	twirl2->setScaleRange(0.1f, 0.4f);
	twirl2->registerAffector(force3);
	twirl2->registerAffector(force4);
	twirl2->addAffector(wind2);
	twirl2->registerAffector(gravity2);
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
	
}

void april_destroy()
{
	delete flame;
	delete bubbles;
	delete vortex;
	delete rain;
	delete quazar_disc;
	delete quazar_jet_top;
	delete quazar_jet_bottom;
	delete twirl1;
	delete twirl2;
	aprilparticle::destroy();
	april::destroy();
}

