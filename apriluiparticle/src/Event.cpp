/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <hltypes/hstring.h>

#include "apriluiparticle.h"
#include "Event.h"

namespace apriluiparticle
{
	hstr Event::ParticleSettingsChanged = "ParticleSettingsChanged";
	hstr Event::ParticleSystemExpired = "ParticleSystemExpired";

	Event::Event()
	{
	}

}
