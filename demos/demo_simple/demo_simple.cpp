/// @file
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifdef _ANDROID
#define RESOURCE_PATH "./"
#else
#define RESOURCE_PATH "../media/"
#endif

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

grect drawRect(0.0f, 0.0f, 1024.0f, 768.0f);
#ifndef _ANDROID
grect viewport = drawRect;
#else
grect viewport(0.0f, 0.0f, 480.0f, 320.0f);
#endif

aprilparticle::System* flame = NULL;
aprilparticle::System* bubbles = NULL;
aprilparticle::System* vortex = NULL;
aprilparticle::System* rain = NULL;
aprilparticle::System* quazar = NULL;
aprilparticle::System* twirl = NULL;
aprilparticle::System* milkyWay = NULL;

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
	april::rendersys->render(april::LineList, grid, 44);
}

bool render(float k)
{
	april::rendersys->clear(true, false);
	static float angle = 0.0f;
	angle += k * 90.0f;

	if (hmodf(angle - k * 90.0f, 90.0f) > hmodf(angle, 90.0f))
	{
		harray<int> counts;
		counts += flame->getParticleCount();
		counts += bubbles->getParticleCount();
		counts += vortex->getParticleCount();
		counts += rain->getParticleCount();
		counts += quazar->getParticleCount();
		counts += twirl->getParticleCount();
		counts += milkyWay->getParticleCount();
		int count = 0;
		foreach (int, it, counts)
		{
			count += (*it);
		}
		counts += count;
		printf("Particles: %s\n", counts.cast<hstr>().join(" ").c_str());
	}
    
	april::rendersys->setPerspective(60.0f, drawRect.getAspect(), 0.1f, 100.0f);
	
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
	milkyWay->update(k);
	
	flame->draw(pos);
	bubbles->draw(pos);
	vortex->draw(pos);
	rain->draw(pos);
	quazar->draw(pos);
	twirl->draw(pos);
	milkyWay->draw(pos);
	
	twirl->getAffector<aprilparticle::Affectors::ForceField>(AFFECTOR_FORCE_1)->setDirection(gvec3(sin(angle * 0.06666667f), 0.0f, cos(angle * 0.03333333f)) * 10.0f);
	twirl->getAffector<aprilparticle::Affectors::ForceField>(AFFECTOR_FORCE_2)->setDirection(gvec3(sin(angle * 0.06666667f), sin(angle * 0.03333333f), 0.0f) * 10.0f);
	twirl->getAffector<aprilparticle::Affectors::ForceField>(AFFECTOR_FORCE_3)->setDirection(gvec3(cos(angle * 0.03333333f), 0.0f, sin(angle * 0.06666667f)) * 10.0f);
	twirl->getAffector<aprilparticle::Affectors::ForceField>(AFFECTOR_FORCE_4)->setDirection(gvec3(sin(angle * 0.03333333f), sin(angle * 0.06666667f), 0.0f) * 10.0f);
	
	return true;
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
	april::createRenderTarget((int)viewport.w, (int)viewport.h, false, "AprilParticle XML Demo");
	aprilparticle::init();
	april::rendersys->getWindow()->setUpdateCallback(render);
	setupGrid(2.0f);
	// setting up every system
	flame = new aprilparticle::System(RESOURCE_PATH "flame.particle");
	flame->load();
	bubbles = new aprilparticle::System(RESOURCE_PATH "bubbles.particle");
	bubbles->load();
	vortex = new aprilparticle::System(RESOURCE_PATH "vortex.particle");
	vortex->load();
	rain = new aprilparticle::System(RESOURCE_PATH "rain.particle");
	rain->load();
	quazar = new aprilparticle::System(RESOURCE_PATH "quazar.particle");
	quazar->load();
	twirl = new aprilparticle::System(RESOURCE_PATH "twirl.particle");
	twirl->load();
	milkyWay = new aprilparticle::System(RESOURCE_PATH "milkyWay.particle");
	milkyWay->load();
}

void april_destroy()
{
	delete flame;
	delete bubbles;
	delete vortex;
	delete rain;
	delete quazar;
	delete twirl;
	delete milkyWay;
	aprilparticle::destroy();
	april::destroy();
}

