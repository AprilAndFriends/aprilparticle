/// @file
/// @author  Boris Mikic
/// @version 1.65
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines all functions for external control.

#ifndef APRILUIPARTICLE_H
#define APRILUIPARTICLE_H

#include <hltypes/hstring.h>

#include "apriluiparticleExport.h"

namespace apriluiparticle
{
	extern hstr logTag;

	apriluiparticleFnExport void init();
	apriluiparticleFnExport void destroy();
	apriluiparticleFnExport bool isEnabled();
	apriluiparticleFnExport void setEnabled(bool value);
	apriluiparticleFnExport hstr getDefaultPath();
	apriluiparticleFnExport void setDefaultPath(chstr value);

	DEPRECATED_ATTRIBUTE apriluiparticleFnExport void setLogFunction(void (*fnptr)(chstr));
	DEPRECATED_ATTRIBUTE apriluiparticleFnExport void log(chstr message, chstr prefix = "[apriluiparticle] ");

};

#endif

