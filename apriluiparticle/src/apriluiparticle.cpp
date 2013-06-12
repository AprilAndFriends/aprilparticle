/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <aprilui/aprilui.h>
#include <hltypes/exception.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "apriluiparticle.h"
#include "Particle.h"
#include "ParticleEmitter.h"
#include "ParticleSpace.h"
#include "ParticleSystem.h"

namespace apriluiparticle
{
	hstr logTag = "apriluiparticle";

	hstr defaultPath = "particles";
	bool enabled = true;

	void init()
	{
		hlog::write(apriluiparticle::logTag, "Initializing AprilUIParticle.");
		APRILUI_REGISTER_OBJECT_TYPE(Particle);
		APRILUI_REGISTER_OBJECT_TYPE(ParticleEmitter);
		APRILUI_REGISTER_OBJECT_TYPE(ParticleSpace);
		APRILUI_REGISTER_OBJECT_TYPE(ParticleSystem);
	}
	
	void destroy()
	{
		hlog::write(apriluiparticle::logTag, "destroying AprilUIParticle.");
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
