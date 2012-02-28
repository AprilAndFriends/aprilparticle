/// @file
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#ifdef _ANDROID
#include <android/log.h>
#endif

#include <hltypes/exception.h>
#include <hltypes/hmap.h>
#include <hltypes/hstring.h>

#include "Affectors.h"
#include "aprilparticle.h"

#define REGISTER_AFFECTOR_TYPE(name) aprilparticle::registerAffectorFactory(#name, &Affectors::name::createInstance)

namespace aprilparticle
{
	static hmap<hstr, Affector* (*)(chstr)> gAffectorFactories;

	void aprilparticle_writelog(chstr message)
	{
#ifndef _ANDROID
		printf("%s\n", message.c_str());
#else
		__android_log_print(ANDROID_LOG_INFO, "aprilparticle", "%s", message.c_str());
#endif
	}
	void (*g_logFunction)(chstr) = aprilparticle_writelog;
	
	extern void initForceField();
    void init()
    {
		log("initializing AprilParticle");
		initForceField(); // so we can shoot the bad guys
		REGISTER_AFFECTOR_TYPE(Attractor);
		REGISTER_AFFECTOR_TYPE(CallbackAffector);
		REGISTER_AFFECTOR_TYPE(ColorChanger);
		REGISTER_AFFECTOR_TYPE(ColorChangerTimed);
		REGISTER_AFFECTOR_TYPE(ForceField);
		REGISTER_AFFECTOR_TYPE(LinearForce);
		REGISTER_AFFECTOR_TYPE(Resizer);
		REGISTER_AFFECTOR_TYPE(ResizerTimed);
		REGISTER_AFFECTOR_TYPE(Revolutor);
		REGISTER_AFFECTOR_TYPE(Rotator);
		REGISTER_AFFECTOR_TYPE(Scaler);
		REGISTER_AFFECTOR_TYPE(ScalerTimed);
    }
    
    void destroy()
    {
		log("destroying AprilParticle");
		gAffectorFactories.clear();
    }

	void setLogFunction(void (*fnptr)(chstr))
	{
		g_logFunction = fnptr;
	}
	
	void log(chstr message, chstr prefix)
	{
		g_logFunction(prefix + message);
	}
	
	void registerAffectorFactory(chstr typeName, Affector* (*factory)(chstr))
	{
		if (gAffectorFactories.has_key(typeName))
		{
			throw hl_exception("Error! Affector Factory '" + typeName + "' already exists!");
		}
		gAffectorFactories[typeName] = factory;
	}

	Affector* createAffector(chstr typeName, chstr name)
	{
		if (gAffectorFactories.has_key(typeName))
		{
			return (*gAffectorFactories[typeName])(name);
		}
		return NULL;
	}
}
