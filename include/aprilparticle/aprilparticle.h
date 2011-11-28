/// @file
/// @author  Boris Mikic
/// @version 1.3
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

#define APRILPARTICLE_VALUE_SEPARATOR ","
#define APRILPARTICLE_RANGE_SEPARATOR ";"
#define APRILPARTICLE_TIMING_SEPARATOR ":"

namespace aprilparticle
{
    aprilparticleFnExport void init();
    aprilparticleFnExport void destroy();
	aprilparticleFnExport void setLogFunction(void (*fnptr)(chstr));
	aprilparticleFnExport void log(chstr message, chstr prefix = "[aprilparticle] ");

};

#endif

