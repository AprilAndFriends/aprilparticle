#include <math.h>

#include <april/main.h>
#include <april/RenderSystem.h>
#include <april/Timer.h>
#include <april/Window.h>
#include <aprilparticle/aprilparticle.h>
#include <aprilparticle/Affectors.h>
#include <aprilparticle/Emitter.h>
#include <aprilparticle/System.h>

#include <gtypes/Matrix3.h>
#include <gtypes/Vector3.h>
#include <hltypes/harray.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

gvec2 screen(1024.0f, 768.0f);
gvec3 up(0.0f, 1.0f, 0.0f);

april::Texture* jet;
april::Texture* plasmaball;
april::Texture* red;
april::Texture* raindrop;
april::Texture* bubble;

aprilparticle::System* flame = NULL;

aprilparticle::Emitter* bubblesEmitter;
aprilparticle::Emitter* vortexEmitter;
aprilparticle::Emitter* rainEmitter;
aprilparticle::Emitter* quazar_disc;
aprilparticle::Emitter* quazar_jet_top;
aprilparticle::Emitter* quazar_jet_bottom;
aprilparticle::Emitter* twirlEmitter1;
aprilparticle::Emitter* twirlEmitter2;

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
    
	april::rendersys->setPerspective(60.0f, screen.x / screen.y, 0.1f, 100.0f);
	
	gvec3 pos(0.0f, 18.0f, 25.0f);
	gmat3 rot;
	rot.setRotation3D(0.0f, 1.0f, 0.0f, angle * 0.2f);
	pos = rot * pos;
	april::rendersys->lookAt(pos, gvec3(0.0f, 0.0f, 0.0f), gvec3(0.0f, 1.0f, 0.0f));
	drawGrid();
	
	flame->update(k);
	bubblesEmitter->update(k);
	vortexEmitter->update(k);
	quazar_disc->update(k);
	quazar_jet_bottom->update(k);
	quazar_jet_top->update(k);
	rainEmitter->update(k);
	twirlEmitter1->update(k);
	twirlEmitter2->update(k);

	force1->setDirection(gvec3(sin(angle * 0.06666667f), 0.0f, cos(angle * 0.03333333f)));
	force2->setDirection(gvec3(sin(angle * 0.06666667f), sin(angle * 0.03333333f), 0.0f));
	force3->setDirection(gvec3(cos(angle * 0.03333333f), 0.0f, sin(angle * 0.06666667f)));
	force4->setDirection(gvec3(sin(angle * 0.03333333f), sin(angle * 0.06666667f), 0.0f));
	
	flame->draw(pos, up);
	bubblesEmitter->draw(pos, up);
	vortexEmitter->draw(pos, up);
	quazar_disc->draw(pos, up);
	quazar_jet_bottom->draw(pos, up);
	quazar_jet_top->draw(pos, up);
	rainEmitter->draw(pos, up);
	twirlEmitter1->draw(pos, up);
	twirlEmitter2->draw(pos, up);
	
	return true;
}

void setupFlame()
{
	flame = new aprilparticle::System();
	april::Texture* fire = april::rendersys->loadTexture("../media/partikl_strange.png");
	flame->registerTexture(fire);
	aprilparticle::Emitter* flameEmitter = new aprilparticle::Emitter();
	flame->registerEmitter(flameEmitter);
	// making the emitter responsible for these affectors
	aprilparticle::Affectors::MultiColorChanger* colorChanger = new aprilparticle::Affectors::MultiColorChanger();
	flameEmitter->registerAffector(colorChanger);
	aprilparticle::Affectors::LinearDirectionalForce* linearforce = new aprilparticle::Affectors::LinearDirectionalForce(gvec3(0.0f, 1.0f, 0.0f), 0.018f);
	flameEmitter->registerAffector(linearforce);
	aprilparticle::Affectors::Rotator* rotator = new aprilparticle::Affectors::Rotator(50.0f);
	flameEmitter->registerAffector(rotator);

	flame->setPosition(-10.0f, 0.0f, 0.0f);
	flameEmitter->setLife(4.0f);
	flameEmitter->setEmissionRate(32.0f);
	flameEmitter->setParticleLimit(128);
	flameEmitter->setBlendMode(april::ADD);
	flameEmitter->setTexture(fire);
	flameEmitter->setType(aprilparticle::Emitter::HollowSphere);
	flameEmitter->setDimensions(2.0f, 4.0f, 2.0f);
	flameEmitter->setScaleRange(2.4f, 3.4f);
	flameEmitter->setAngleRange(0.0f, 360.0f);
	hmap<float, april::Color> colors;
	colors[0.0f] = april::Color(0xFF000000);
	colors[0.1f] = april::Color(0xFF6432F3);
	colors[0.4f] = april::Color(0xFF7F3FAF);
	colors[0.75f] = april::Color(0x7F7F7FAF);
	colors[1.0f] = april::Color(0x7F7F7F00);
	colorChanger->setColorTimings(colors);
}

void setupBubbles()
{
	aprilparticle::Affectors::MultiColorChanger* colorChanger = new aprilparticle::Affectors::MultiColorChanger();
	aprilparticle::Affectors::DirectionalForceField* directionalForce = new aprilparticle::Affectors::DirectionalForceField(gvec3(0.0f, 1.0f, 0.0f), gvec3(0.0f, 1.0f, 0.0f), 4.0f);
	bubblesEmitter = new aprilparticle::Emitter();

	bubblesEmitter->setLife(6.0f);
	bubblesEmitter->setEmissionRate(50.0f);
	bubblesEmitter->setParticleLimit(300);
	bubblesEmitter->setBlendMode(april::DEFAULT);
	bubblesEmitter->setTexture(bubble);
	bubblesEmitter->setType(aprilparticle::Emitter::Cylinder);
	bubblesEmitter->setDimensions(5.0f, 4.0f, 5.0f);
	bubblesEmitter->setPosition(0.0f, 0.0f, 0.0f);
	bubblesEmitter->setScaleRange(0.4f, 0.8f);
	hmap<float, april::Color> colors;
	colors[0.0f] = april::Color(0xFFFFFF00);
	colors[0.04f] = april::Color(0xFFFFFFFF);
	colors[0.99f] = april::Color(0xFFFFFFFF);
	colors[1.0f] = april::Color(0xFFFFFF00);
	colorChanger->setColorTimings(colors);
	bubblesEmitter->registerAffector(colorChanger);
	bubblesEmitter->registerAffector(directionalForce);
}

void setupVortex()
{
	aprilparticle::Affectors::Attractor* attractor1 = new aprilparticle::Affectors::Attractor(gvec3(10.0f, 0.0f, 0.0f), 2.0f);
	aprilparticle::Affectors::Swirl* swirl = new aprilparticle::Affectors::Swirl(gvec3(10.0f, 0.0f, 0.0f), gvec3(0.0f, 1.0f, 0.0f), 0.2f, 8.0f);
	vortexEmitter = new aprilparticle::Emitter();

	vortexEmitter->setLife(2.0f);
	vortexEmitter->setEmissionRate(200.0f);
	vortexEmitter->setParticleLimit(800);
	vortexEmitter->setBlendMode(april::ADD);
	vortexEmitter->setTexture(red);
	vortexEmitter->setType(aprilparticle::Emitter::Sphere);
	vortexEmitter->setDimensions(8.0f, 0.0f, 8.0f);
	vortexEmitter->setPosition(10.0f, 0.0f, 0.0f);
	vortexEmitter->setScale(0.4f);
	vortexEmitter->registerAffector(swirl);
	vortexEmitter->registerAffector(attractor1);
}

void setupRain()
{
	aprilparticle::Affectors::LinearDirectionalForce* gravity = new aprilparticle::Affectors::LinearDirectionalForce(gvec3(0.0f, 1.0f, 0.0f), -0.012f);
	rainEmitter = new aprilparticle::Emitter();

	rainEmitter->setLife(4.0f);
	rainEmitter->setEmissionRate(60.0f);
	rainEmitter->setParticleLimit(240);
	rainEmitter->setBlendMode(april::ADD);
	rainEmitter->setTexture(raindrop);
	rainEmitter->setType(aprilparticle::Emitter::Box);
	rainEmitter->setDimensions(6.0f, 0.0f, 6.0f);
	rainEmitter->setPosition(-10.0f, -10.0f, 6.0f);
	rainEmitter->setScaleRange(0.1f, 0.4f);
	rainEmitter->registerAffector(gravity);
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
	quazar_disc = new aprilparticle::Emitter();
	quazar_jet_top = new aprilparticle::Emitter();
	quazar_jet_bottom = new aprilparticle::Emitter();

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
	
	quazar_disc->setType(aprilparticle::Emitter::HollowSphere);
	quazar_disc->setDimensions(1.0f, 6.0f, 6.0f);
	quazar_disc->setPosition(0.0f, 0.0f, 10.0f);
	quazar_disc->setScale(0.3f);
	quazar_disc->registerAffector(gravity);
	quazar_disc->registerAffector(spin);
	
	quazar_jet_bottom->setType(aprilparticle::Emitter::Sphere);
	quazar_jet_bottom->setDimensions(1.0f, 1.0f, 1.0f);
	quazar_jet_bottom->setPosition(0.0f, 0.0f, 10.0f);
	quazar_jet_bottom->setScale(0.3f);
	quazar_jet_bottom->registerAffector(bottom);
	quazar_jet_bottom->registerAffector(inverseGravity1);
	quazar_jet_bottom->registerAffector(attractionPointBottom);
	
	quazar_jet_top->setType(aprilparticle::Emitter::Sphere);
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
	twirlEmitter1 = new aprilparticle::Emitter();
	twirlEmitter2 = new aprilparticle::Emitter();

	twirlEmitter1->setLife(6.0f);
	twirlEmitter1->setEmissionRate(100.0f);
	twirlEmitter1->setParticleLimit(600);
	twirlEmitter1->setBlendMode(april::ADD);
	twirlEmitter1->setTexture(red);
	twirlEmitter1->setType(aprilparticle::Emitter::Point);
	twirlEmitter1->setPosition(10.0f, 0.0f, -10.0f);
	twirlEmitter1->setScaleRange(0.1f, 0.4f);
	twirlEmitter1->registerAffector(force1);
	twirlEmitter1->registerAffector(force2);
	twirlEmitter1->registerAffector(wind1);
	twirlEmitter1->registerAffector(gravity1);
	
	twirlEmitter2->setLife(6.0f);
	twirlEmitter2->setEmissionRate(100.0f);
	twirlEmitter2->setParticleLimit(600);
	twirlEmitter2->setBlendMode(april::ADD);
	twirlEmitter2->setTexture(plasmaball);
	twirlEmitter2->setType(aprilparticle::Emitter::Point);
	twirlEmitter2->setPosition(10.0f, 0.0f, -10.0f);
	twirlEmitter2->setScaleRange(0.1f, 0.4f);
	twirlEmitter2->registerAffector(force3);
	twirlEmitter2->registerAffector(force4);
	twirlEmitter2->addAffector(wind2);
	twirlEmitter2->registerAffector(gravity2);
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
	april::createRenderTarget((int)screen.x, (int)screen.y, false, "AprilParticle Demo");
	aprilparticle::init();
	april::rendersys->getWindow()->setUpdateCallback(render);
	setupGrid(2.0f);
	
	red = april::rendersys->loadTexture("../media/red_particle.png");
	bubble = april::rendersys->loadTexture("../media/bubble.png");
	plasmaball = april::rendersys->loadTexture("../media/plasma_ball.png");
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
	/*
	delete flameEmitter;
	delete bubblesEmitter;
	delete vortexEmitter;
	delete rainEmitter;
	delete quazar_disc;
	delete quazar_jet_top;
	delete quazar_jet_bottom;
	delete twirlEmitter1;
	delete twirlEmitter2;
	*/
	delete flame;
	aprilparticle::destroy();
	april::destroy();
}

