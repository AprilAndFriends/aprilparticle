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

#define AFFECTOR_FORCE_1 "force1"
#define AFFECTOR_FORCE_2 "force2"
#define AFFECTOR_FORCE_3 "force3"
#define AFFECTOR_FORCE_4 "force4"

gvec2 screen(1024.0f, 768.0f);
gvec3 up(0.0f, 1.0f, 0.0f);

april::Texture* redParticle;
april::Texture* greenParticle;

aprilparticle::System* flame = NULL;
aprilparticle::System* bubbles = NULL;
aprilparticle::System* vortex = NULL;
aprilparticle::System* rain = NULL;
aprilparticle::System* quazar = NULL;
aprilparticle::System* twirl = NULL;

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
	bubbles->update(k);
	vortex->update(k);
	rain->update(k);
	quazar->update(k);
	twirl->update(k);
	
	flame->draw(pos, up);
	bubbles->draw(pos, up);
	vortex->draw(pos, up);
	rain->draw(pos, up);
	quazar->draw(pos, up);
	twirl->draw(pos, up);
	
	twirl->getAffector<aprilparticle::Affectors::DirectionalForceField>(AFFECTOR_FORCE_1)->setDirection(gvec3(sin(angle * 0.06666667f), 0.0f, cos(angle * 0.03333333f)));
	twirl->getAffector<aprilparticle::Affectors::DirectionalForceField>(AFFECTOR_FORCE_2)->setDirection(gvec3(sin(angle * 0.06666667f), sin(angle * 0.03333333f), 0.0f));
	twirl->getAffector<aprilparticle::Affectors::DirectionalForceField>(AFFECTOR_FORCE_3)->setDirection(gvec3(cos(angle * 0.03333333f), 0.0f, sin(angle * 0.06666667f)));
	twirl->getAffector<aprilparticle::Affectors::DirectionalForceField>(AFFECTOR_FORCE_4)->setDirection(gvec3(sin(angle * 0.03333333f), sin(angle * 0.06666667f), 0.0f));
	
	return true;
}

void setupFlame()
{
	flame = new aprilparticle::System();
	aprilparticle::Emitter* flameEmitter = new aprilparticle::Emitter();
	flame->registerEmitter(flameEmitter);
	// making the emitter responsible for the texture
	flameEmitter->registerTexture(april::rendersys->loadTexture("../media/partikl_strange.png"));
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
	bubbles = new aprilparticle::System();
	aprilparticle::Emitter* bubblesEmitter = new aprilparticle::Emitter();
	bubbles->registerEmitter(bubblesEmitter);
	// making the emitter responsible for the texture
	bubblesEmitter->registerTexture(april::rendersys->loadTexture("../media/bubble.png"));
	// making the emitter responsible for these affectors
	aprilparticle::Affectors::MultiColorChanger* colorChanger = new aprilparticle::Affectors::MultiColorChanger();
	bubblesEmitter->registerAffector(colorChanger);
	aprilparticle::Affectors::DirectionalForceField* directionalForce = new aprilparticle::Affectors::DirectionalForceField(gvec3(0.0f, 1.0f, 0.0f), gvec3(0.0f, 1.0f, 0.0f), 4.0f);
	bubblesEmitter->registerAffector(directionalForce);

	bubblesEmitter->setLife(6.0f);
	bubblesEmitter->setEmissionRate(50.0f);
	bubblesEmitter->setParticleLimit(300);
	bubblesEmitter->setBlendMode(april::DEFAULT);
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
}

void setupVortex()
{
	vortex = new aprilparticle::System();
	aprilparticle::Emitter* vortexEmitter = new aprilparticle::Emitter();
	vortex->registerEmitter(vortexEmitter);
	// texture is shared among several particle systems
	vortexEmitter->setTexture(redParticle);
	// making the emitter responsible for these affectors
	aprilparticle::Affectors::Swirl* swirl = new aprilparticle::Affectors::Swirl(gvec3(10.0f, 0.0f, 0.0f), gvec3(0.0f, 1.0f, 0.0f), 0.2f, 8.0f);
	vortexEmitter->registerAffector(swirl);
	aprilparticle::Affectors::Attractor* attractor = new aprilparticle::Affectors::Attractor(gvec3(10.0f, 0.0f, 0.0f), 2.0f);
	vortexEmitter->registerAffector(attractor);

	vortexEmitter->setLife(2.0f);
	vortexEmitter->setEmissionRate(200.0f);
	vortexEmitter->setParticleLimit(800);
	vortexEmitter->setBlendMode(april::ADD);
	vortexEmitter->setType(aprilparticle::Emitter::Sphere);
	vortexEmitter->setDimensions(8.0f, 0.0f, 8.0f);
	vortexEmitter->setPosition(10.0f, 0.0f, 0.0f);
	vortexEmitter->setScale(0.4f);
}

void setupRain()
{
	rain = new aprilparticle::System();
	aprilparticle::Emitter* rainEmitter = new aprilparticle::Emitter();
	rain->registerEmitter(rainEmitter);
	// making the emitter responsible for the texture
	rainEmitter->registerTexture(april::rendersys->loadTexture("../media/rainDrop.png"));
	// making the emitter responsible for these affectors
	aprilparticle::Affectors::LinearDirectionalForce* gravity = new aprilparticle::Affectors::LinearDirectionalForce(gvec3(0.0f, 1.0f, 0.0f), -0.012f);
	rainEmitter->registerAffector(gravity);

	rainEmitter->setLife(4.0f);
	rainEmitter->setEmissionRate(60.0f);
	rainEmitter->setParticleLimit(240);
	rainEmitter->setBlendMode(april::ADD);
	rainEmitter->setType(aprilparticle::Emitter::Box);
	rainEmitter->setDimensions(6.0f, 0.0f, 6.0f);
	rainEmitter->setPosition(-10.0f, -10.0f, 6.0f);
	rainEmitter->setScaleRange(0.1f, 0.4f);
}

void setupQuazar()
{
	quazar = new aprilparticle::System();
	aprilparticle::Emitter* discEmitter = new aprilparticle::Emitter();
	quazar->registerEmitter(discEmitter);
	aprilparticle::Emitter* jetTopEmitter = new aprilparticle::Emitter();
	quazar->registerEmitter(jetTopEmitter);
	aprilparticle::Emitter* jetBottomEmitter = new aprilparticle::Emitter();
	quazar->registerEmitter(jetBottomEmitter);
	// making the system responsible for the texture
	april::Texture* blueParticle = april::rendersys->loadTexture("../media/blue_particle.png");
	quazar->registerTexture(blueParticle);
	jetTopEmitter->setTexture(blueParticle);
	jetBottomEmitter->setTexture(blueParticle);
	// texture is shared among several particle systems
	discEmitter->setTexture(greenParticle);
	// making the emitters responsible for these affectors
	aprilparticle::Affectors::Attractor* gravity = new aprilparticle::Affectors::Attractor(gvec3(0.0f, 0.0f, 10.0f), 2.0f);
	discEmitter->registerAffector(gravity);
	aprilparticle::Affectors::Swirl* spin = new aprilparticle::Affectors::Swirl(gvec3(0.0f, 0.0f, 10.0f), gvec3(1.0f, 0.0f, 0.0f), 0.1f, 8.0f);
	discEmitter->registerAffector(spin);
	aprilparticle::Affectors::LinearDirectionalForce* top = new aprilparticle::Affectors::LinearDirectionalForce(gvec3(1.0f, 0.0f, 0.0f), 0.024f);
	jetTopEmitter->registerAffector(top);
	aprilparticle::Affectors::Attractor* attractionPointTop = new aprilparticle::Affectors::Attractor(gvec3(10.0f, 0.0f, 10.0f), 6.0f);
	jetTopEmitter->registerAffector(attractionPointTop);
	aprilparticle::Affectors::LinearDirectionalForce* bottom = new aprilparticle::Affectors::LinearDirectionalForce(gvec3(-1.0f, 0.0f, 0.0f), 0.024f);
	jetBottomEmitter->registerAffector(bottom);
	aprilparticle::Affectors::Attractor* attractionPointBottom = new aprilparticle::Affectors::Attractor(gvec3(-10.0f, 0.0f, 10.0f), 6.0f);
	jetBottomEmitter->registerAffector(attractionPointBottom);
	// making the system responsible for these affectors, because they are share between more than one emitter in order to save memory
	aprilparticle::Affectors::Attractor* inverseGravity = new aprilparticle::Affectors::Attractor(gvec3(0.0f, 0.0f, 10.0f), -4.0f);
	quazar->registerAffector(inverseGravity);
	jetTopEmitter->addAffector(inverseGravity);
	jetBottomEmitter->addAffector(inverseGravity);

	discEmitter->setLife(4.0f);
	discEmitter->setEmissionRate(240.0f);
	discEmitter->setParticleLimit(960);
	discEmitter->setBlendMode(april::ADD);
	discEmitter->setType(aprilparticle::Emitter::HollowSphere);
	discEmitter->setDimensions(1.0f, 6.0f, 6.0f);
	discEmitter->setPosition(0.0f, 0.0f, 10.0f);
	discEmitter->setScale(0.3f);
	
	jetTopEmitter->setLife(4.0f);
	jetTopEmitter->setEmissionRate(120.0f);
	jetTopEmitter->setParticleLimit(480);
	jetTopEmitter->setBlendMode(april::ADD);
	jetTopEmitter->setType(aprilparticle::Emitter::Sphere);
	jetTopEmitter->setDimensions(1.0f, 1.0f, 1.0f);
	jetTopEmitter->setPosition(0.0f, 0.0f, 10.0f);
	jetTopEmitter->setScale(0.3f);
	
	jetBottomEmitter->setLife(4.0f);
	jetBottomEmitter->setEmissionRate(120.0f);
	jetBottomEmitter->setParticleLimit(480);
	jetBottomEmitter->setBlendMode(april::ADD);
	jetBottomEmitter->setType(aprilparticle::Emitter::Sphere);
	jetBottomEmitter->setDimensions(1.0f, 1.0f, 1.0f);
	jetBottomEmitter->setPosition(0.0f, 0.0f, 10.0f);
	jetBottomEmitter->setScale(0.3f);
}

void setupTwirl()
{
	twirl = new aprilparticle::System();
	aprilparticle::Emitter* left = new aprilparticle::Emitter();
	twirl->registerEmitter(left);
	aprilparticle::Emitter* right = new aprilparticle::Emitter();
	twirl->registerEmitter(right);
	// texture is shared among several particle systems
	left->setTexture(redParticle);
	right->setTexture(greenParticle);
	// making the system responsible for these affectors
	aprilparticle::Affectors::LinearDirectionalForce* wind = new aprilparticle::Affectors::LinearDirectionalForce(gvec3(-1.0f, 0.0f, 0.0f), 0.012f);
	twirl->registerAffector(wind);
	left->addAffector(wind);
	right->addAffector(wind);
	aprilparticle::Affectors::Attractor* gravity = new aprilparticle::Affectors::Attractor(gvec3(0.0f, 0.0f, -10.0f), 7.0f);
	twirl->registerAffector(gravity);
	left->addAffector(gravity);
	right->addAffector(gravity);
	// making the emitters responsible for these affectors
	aprilparticle::Affectors::DirectionalForceField* force1 = new aprilparticle::Affectors::DirectionalForceField(gvec3(10.0f, 0.0f, -10.0f), gvec3(-1.0f, 1.0f, 0.0f), 4.0f);
	left->registerAffector(force1);
	aprilparticle::Affectors::DirectionalForceField* force2 = new aprilparticle::Affectors::DirectionalForceField(gvec3(10.0f, 4.0f, -10.0f), gvec3(1.0f, 1.0f, 0.0f), 4.0f);
	left->registerAffector(force2);
	aprilparticle::Affectors::DirectionalForceField* force3 = new aprilparticle::Affectors::DirectionalForceField(gvec3(10.0f, 0.0f, -10.0f), gvec3(-1.0f, 1.0f, 0.0f), 4.0f);
	right->registerAffector(force3);
	aprilparticle::Affectors::DirectionalForceField* force4 = new aprilparticle::Affectors::DirectionalForceField(gvec3(10.0f, 4.0f, -10.0f), gvec3(1.0f, 1.0f, 0.0f), 4.0f);
	right->registerAffector(force4);
	// the affectors are named so they can be accessed from the outside
	force1->setName(AFFECTOR_FORCE_1);
	force2->setName(AFFECTOR_FORCE_2);
	force3->setName(AFFECTOR_FORCE_3);
	force4->setName(AFFECTOR_FORCE_4);

	left->setLife(6.0f);
	left->setEmissionRate(100.0f);
	left->setParticleLimit(600);
	left->setBlendMode(april::ADD);
	left->setType(aprilparticle::Emitter::Point);
	left->setPosition(10.0f, 0.0f, -10.0f);
	left->setScaleRange(0.1f, 0.4f);
	
	right->setLife(6.0f);
	right->setEmissionRate(100.0f);
	right->setParticleLimit(600);
	right->setBlendMode(april::ADD);
	right->setType(aprilparticle::Emitter::Point);
	right->setPosition(10.0f, 0.0f, -10.0f);
	right->setScaleRange(0.1f, 0.4f);
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
	// textures used by more than one system
	redParticle = april::rendersys->loadTexture("../media/red_particle.png");
	greenParticle = april::rendersys->loadTexture("../media/green_particle.png");
	// setting up every system
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
	delete quazar;
	delete twirl;
	delete redParticle;
	delete greenParticle;
	aprilparticle::destroy();
	april::destroy();
}

