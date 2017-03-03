/// @file
/// @version 2.03
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#ifndef _ANDROID
#ifndef _WINRT
#define RESOURCE_PATH "../../demos/media/"
#else
#define RESOURCE_PATH "media/"
#endif
#elif defined(__APPLE__)
#define RESOURCE_PATH "media/"
#else
#define RESOURCE_PATH "./"
#endif

#include <stdio.h>

#ifdef __APPLE__
#include <CoreFoundation/CoreFoundation.h>
#endif

#include <april/april.h>
#include <april/KeyboardDelegate.h>
#include <april/main.h>
#include <april/MouseDelegate.h>
#include <april/Platform.h>
#include <april/RenderSystem.h>
#include <april/UpdateDelegate.h>
#include <april/Window.h>
#include <aprilui/aprilui.h>
#include <aprilui/Animator.h>
#include <aprilui/aprilui.h>
#include <aprilui/Dataset.h>
#include <aprilui/Objects.h>
#include <aprilparticle/aprilparticle.h>
#include <apriluiparticle/apriluiparticle.h>
#include <apriluiparticle/System.h>
#include <atres/atres.h>
#include <atres/Renderer.h>
#include <gtypes/Vector2.h>

#define LOG_TAG "demo_aprilui"

grect drawRect(0.0f, 0.0f, 800.0f, 600.0f);
grect viewport = drawRect;

aprilui::Dataset* dataset = NULL;

class UpdateDelegate : public april::UpdateDelegate
{
public:
	UpdateDelegate()
	{
		this->firstFrame = true;
	}
	
	bool onUpdate(float timeDelta)
	{
		if (this->firstFrame)
		{
			timeDelta = 0.0f;
			this->firstFrame = false;
		}
		april::rendersys->clear();
		april::rendersys->setOrthoProjection(drawRect);
		aprilui::updateCursorPosition();
		dataset->update(timeDelta);
		dataset->draw();
		return true;
	}
	
protected:
	bool firstFrame;

} updateDelegate;

class KeyboardDelegate : public april::KeyboardDelegate
{
	void onKeyDown(april::Key keycode)
	{
		if (keycode == april::Key::Return)
		{
			dataset->unload();
			dataset->load();
		}
		else if (keycode == april::AK_BACK)
		{
			apriluiparticle::System* particle = dataset->getObject<apriluiparticle::System*>("segmented");
			particle->stopSystem();
			particle->load(particle->getFilename());
		}
		aprilui::onKeyDown(keycode);
	}
	
	void onKeyUp(april::Key keyCode)
	{
		aprilui::onKeyUp(keyCode);
	}
	
	void onChar(unsigned int charCode)
	{
		aprilui::onChar(charCode);
	}

} keyboardDelegate;

class MouseDelegate : public april::MouseDelegate
{
	void onMouseDown(april::Key button)
	{
		aprilui::onMouseDown(button);
	}

	void onMouseUp(april::Key button)
	{
		aprilui::onMouseUp(button);
	}

	void onMouseMove()
	{
		aprilui::onMouseMove();
	}

	void onMouseScroll(float x, float y)
	{
		aprilui::onMouseScroll(x, y);
	}

} mouseDelegate;

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
	try
	{
#if defined(_ANDROID) || defined(_IOS)
		drawRect.setSize(april::getSystemInfo().displayResolution);
#endif
		april::init(april::RenderSystemType::Default, april::WindowType::Default);
		april::createRenderSystem();
		april::Window::Options options;
		options.fpsCounter = true;
		april::createWindow((int)drawRect.w, (int)drawRect.h, false, "AprilParticle Demo AprilUI", options);
		atres::init();
		aprilui::init();
		aprilparticle::init();
		aprilparticle::setUseCache(true);
		apriluiparticle::init();
		april::window->setUpdateDelegate(&updateDelegate);
		april::window->setKeyboardDelegate(&keyboardDelegate);
		april::window->setMouseDelegate(&mouseDelegate);
		apriluiparticle::setDefaultPath("");
		dataset = new aprilui::Dataset(RESOURCE_PATH "demo_aprilui.dts");
		dataset->load();
#ifdef _DEBUG
		//aprilui::setDebugEnabled(true);
#endif
	}
	catch (hexception& e)
	{
		hlog::error(LOG_TAG, e.getMessage());
	}
}

void april_destroy()
{
	try
	{
		delete dataset;
		apriluiparticle::destroy();
		aprilparticle::destroy();
		aprilui::destroy();
		atres::destroy();
		april::destroy();
	}
	catch (hexception& e)
	{
		hlog::error(LOG_TAG, e.getMessage());
	}
}
