/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef __ANDROID__
	#ifndef _UWP
		#define RESOURCE_PATH "../../demos/media/"
	#else
		#define RESOURCE_PATH "media/"
	#endif
#elif defined(__APPLE__)
	#define RESOURCE_PATH "media/"
#else
	#define RESOURCE_PATH "./"
#endif

#include <april/april.h>
#include <april/main.h>
#include <april/Platform.h>
#include <april/RenderSystem.h>
#include <april/Timer.h>
#include <april/UpdateDelegate.h>
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

grectf drawRect(0.0f, 0.0f, 800.0f, 600.0f);
grectf viewport = drawRect;

aprilparticle::System* flame = NULL;
aprilparticle::System* bubbles = NULL;
aprilparticle::System* vortex = NULL;
aprilparticle::System* rain = NULL;
aprilparticle::System* quazar = NULL;
aprilparticle::System* milkyWay = NULL;
aprilparticle::System* twirl = NULL;

april::ColoredVertex grid[44];

void setupGrid(float spacing)
{
	int i = 0;
	for (float s = -5 * spacing; s <= 5 * spacing; i++, s += spacing)
	{
		grid[i * 4 + 0].set(-5 * spacing, 0.0f, s);
		grid[i * 4 + 0].color = 0xFFFFFFFF;
		grid[i * 4 + 1].set(5 * spacing, 0.0f, s);
		grid[i * 4 + 1].color = 0xFFFFFFFF;
		grid[i * 4 + 2].set(s, 0.0f, -5 * spacing);
		grid[i * 4 + 2].color = 0xFFFFFFFF;
		grid[i * 4 + 3].set(s, 0.0f, 5 * spacing);
		grid[i * 4 + 3].color = 0xFFFFFFFF;
	}
}

void drawGrid()
{
	april::rendersys->render(april::RenderOperation::LineList, grid, 44);
}

class UpdateDelegate : public april::UpdateDelegate
{
public:
	UpdateDelegate() : angle(0.0f), count(0)
	{
	}

	bool onUpdate(float timeDelta)
	{
		april::rendersys->clear();
		this->angle += timeDelta * 90.0f;
		
		if (hmodf(this->angle - timeDelta * 90.0f, 90.0f) > hmodf(this->angle, 90.0f))
		{
			this->counts.clear();
			this->counts += flame->getParticleCount();
			this->counts += bubbles->getParticleCount();
			this->counts += vortex->getParticleCount();
			this->counts += rain->getParticleCount();
			this->counts += quazar->getParticleCount();
			this->counts += milkyWay->getParticleCount();
			this->counts += twirl->getParticleCount();
			this->count = 0;
			foreach (int, it, this->counts)
			{
				this->count += (*it);
			}
			this->counts += this->count;
			printf("Particles: %s\n", this->counts.cast<hstr>().joined(' ').cStr());
		}
		
		april::rendersys->setPerspective(60.0f, 1 / drawRect.getAspect(), 0.1f, 100.0f);
		
		gvec3f pos(0.0f, 18.0f, 25.0f);
		gmat3 rot;
		rot.setRotation3D(0.0f, 1.0f, 0.0f, this->angle * 0.2f);
		pos = rot * pos;
		april::rendersys->lookAt(pos, gvec3f(0.0f, 0.0f, 0.0f), gvec3f(0.0f, 1.0f, 0.0f));
		drawGrid();
		
		twirl->getAffector<aprilparticle::Affectors::ForceField>(AFFECTOR_FORCE_1)->setDirection(gvec3f(sin(this->angle * 0.06666667f), 0.0f, cos(this->angle * 0.03333333f)) * 10.0f);
		twirl->getAffector<aprilparticle::Affectors::ForceField>(AFFECTOR_FORCE_2)->setDirection(gvec3f(sin(this->angle * 0.06666667f), sin(this->angle * 0.03333333f), 0.0f) * 10.0f);
		twirl->getAffector<aprilparticle::Affectors::ForceField>(AFFECTOR_FORCE_3)->setDirection(gvec3f(cos(this->angle * 0.03333333f), 0.0f, sin(this->angle * 0.06666667f)) * 10.0f);
		twirl->getAffector<aprilparticle::Affectors::ForceField>(AFFECTOR_FORCE_4)->setDirection(gvec3f(sin(this->angle * 0.03333333f), sin(this->angle * 0.06666667f), 0.0f) * 10.0f);
		
		flame->update(timeDelta);
		bubbles->update(timeDelta);
		vortex->update(timeDelta);
		rain->update(timeDelta);
		quazar->update(timeDelta);
		milkyWay->update(timeDelta);
		twirl->update(timeDelta);
		
		flame->draw(pos);
		bubbles->draw(pos);
		vortex->draw(pos);
		rain->draw(pos);
		quazar->draw(pos);
		milkyWay->draw(pos);
		twirl->draw(pos);
		
		return true;
	}

protected:
	float angle;
	harray<int> counts;
	int count;

} updateDelegate;

void __aprilApplicationInit()
{
#ifdef __APPLE__
	// On MacOSX, the current working directory is not set by
	// the Finder, since you are expected to use Core Foundation
	// or ObjC APIs to find files. 
	// So, when porting you probably want to set the current working
	// directory to something sane (e.g. .../Resources/ in the app
	// bundle).
	// In this case, we set it to parent of the .app bundle.
	{

		CFURLRef url = CFBundleCopyBundleURL(CFBundleGetMainBundle());
		CFStringRef path = CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle);
		// let's hope chdir() will be happy with utf8 encoding
		const char* cpath = CFStringGetCStringPtr(path, kCFStringEncodingUTF8);
		char* cpath_alloc = NULL;
		if (cpath == NULL)
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
#if defined(__ANDROID__) || defined(_IOS)
	drawRect.setSize(april::getSystemInfo().displayResolution);
#endif
	april::init(april::RenderSystemType::Default, april::WindowType::Default);
	april::createRenderSystem();
	april::Window::Options options;
	options.fpsCounter = true;
	april::createWindow((int)drawRect.w, (int)drawRect.h, false, "AprilParticle XML Demo", options);
	aprilparticle::init();
	april::window->setUpdateDelegate(&updateDelegate);
	setupGrid(2.0f);
	// setting up every system
	flame = aprilparticle::loadSystem(RESOURCE_PATH "flame.particle");
	flame->load();
	bubbles = aprilparticle::loadSystem(RESOURCE_PATH "bubbles.particle");
	bubbles->load();
	vortex = aprilparticle::loadSystem(RESOURCE_PATH "vortex.particle");
	vortex->load();
	rain = aprilparticle::loadSystem(RESOURCE_PATH "rain.particle");
	rain->load();
	quazar = aprilparticle::loadSystem(RESOURCE_PATH "quazar.particle");
	quazar->load();
	milkyWay = aprilparticle::loadSystem(RESOURCE_PATH "milkyWay.particle");
	milkyWay->load();
	twirl =  aprilparticle::loadSystem(RESOURCE_PATH "twirl.particle");
	twirl->load();
}

void __aprilApplicationDestroy()
{
	delete flame;
	delete bubbles;
	delete vortex;
	delete rain;
	delete quazar;
	delete milkyWay;
	delete twirl;
	aprilparticle::destroy();
	april::destroy();
}

