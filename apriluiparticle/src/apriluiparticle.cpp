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

#include <aprilui/aprilui.h>
#include <hltypes/exception.h>
#include <hltypes/hstring.h>

#include "apriluiparticle.h"
#include "Particle.h"

namespace apriluiparticle
{
	hstr defaultPath = "particles";
	bool enabled = true;

	void apriluiparticle_writelog(chstr message)
	{
#ifndef _ANDROID
		printf("%s\n", message.c_str());
#else
		__android_log_print(ANDROID_LOG_INFO, "apriluiparticle", "%s", message.c_str());
#endif
	}
	void (*g_logFunction)(chstr) = apriluiparticle_writelog;
	
	void init()
    {
		log("initializing AprilUIParticle");
		APRILUI_REGISTER_OBJECT_TYPE(Particle);
    }
    
    void destroy()
    {
		log("destroying AprilUIParticle");
    }

	void setLogFunction(void (*fnptr)(chstr))
	{
		g_logFunction = fnptr;
	}
	
	void log(chstr message, chstr prefix)
	{
		g_logFunction(prefix + message);
	}
	
	hstr getDefaultPath()
	{
		return defaultPath;
	}
	
	void setDefaultPath(chstr value)
	{
		defaultPath = value;
	}

	bool isEnabled()
	{
		return enabled;
	}
	
	void setEnabled(bool value)
	{
		enabled = value;
	}

}
