/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines all functions for external control.

#ifndef APRILPARTICLE_H
#define APRILPARTICLE_H

#include <hltypes/hstring.h>

#include "aprilparticleExport.h"

#define APRILPARTICLE_REGISTER_AFFECTOR_TYPE(name) aprilparticle::registerAffectorFactory(#name, &name::createInstance)

namespace aprilparticle
{
	static const char SeparatorValue = ' ';
	static const char SeparatorVector = ',';
	static const char SeparatorRange = ';';
	static const char SeparatorTiming = ':';

	class Affector;
	class System;
	class Texture;

	extern hstr logTag;

	aprilparticleFnExport void init();
	aprilparticleFnExport void destroy();
	aprilparticleFnExport bool getUseCache();
	aprilparticleFnExport void setUseCache(bool value);
	aprilparticleFnExport void registerAffectorFactory(chstr typeName, Affector* (*factory)(chstr));
	aprilparticleFnExport Affector* createAffector(chstr type, chstr name = "");
	aprilparticleFnExport aprilparticle::Texture* loadTexture(chstr filename, bool cached = false);
	aprilparticleFnExport void clearCache();
	aprilparticleFnExport System* loadSystem(chstr filename, chstr name = "");

};

#endif

