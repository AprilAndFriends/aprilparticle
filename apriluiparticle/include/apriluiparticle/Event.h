/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
/// 
/// @section DESCRIPTION
/// 
/// Defines an event.

#ifndef APRILUIPARTICLE_EVENT_H
#define APRILUIPARTICLE_EVENT_H

#include <hltypes/hstring.h>

#include "apriluiparticleExport.h"

namespace apriluiparticle
{
	class apriluiparticleExport Event
	{
	public:
		static hstr ParticleSettingsChanged;
		static hstr ParticleSystemExpired;

	private:
		Event();
		~Event();

	};

}

#endif
