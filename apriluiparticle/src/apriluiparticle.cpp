/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <aprilui/aprilui.h>
#include <hltypes/hexception.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>
#include <hltypes/hversion.h>

#include "apriluiparticle.h"
#include "Emitter.h"
#include "Particle.h"
#include "Space.h"
#include "System.h"

namespace apriluiparticle
{
	hstr logTag = "apriluiparticle";

	static hversion version(2, 3, 0);

	static hstr defaultPath = "particles";
	static bool enabled = true;

	void init()
	{
		hlog::write(logTag, "Initializing AprilUIParticle: " + version.toString());
		APRILUI_REGISTER_OBJECT_TYPE(Particle);
		APRILUI_REGISTER_OBJECT_TYPE(Emitter);
		APRILUI_REGISTER_OBJECT_TYPE(Space);
		APRILUI_REGISTER_OBJECT_TYPE(System);
	}
	
	void destroy()
	{
		hlog::write(logTag, "destroying AprilUIParticle.");
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
