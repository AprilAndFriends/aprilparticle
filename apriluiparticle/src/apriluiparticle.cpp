/// @file
/// @version 2.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <aprilui/aprilui.h>
#include <hltypes/exception.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "apriluiparticle.h"
#include "Emitter.h"
#include "Particle.h"
#include "Space.h"
#include "System.h"

namespace apriluiparticle
{
	hstr logTag = "apriluiparticle";

	hstr defaultPath = "particles";
	bool enabled = true;

	void init()
	{
		hlog::write(apriluiparticle::logTag, "Initializing AprilUIParticle.");
		APRILUI_REGISTER_OBJECT_TYPE(Particle);
		APRILUI_REGISTER_OBJECT_TYPE(Emitter);
		APRILUI_REGISTER_OBJECT_TYPE(Space);
		APRILUI_REGISTER_OBJECT_TYPE(System);
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
