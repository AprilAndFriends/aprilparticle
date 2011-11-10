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

#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/hstring.h>

#include "aprilparticleExport.h"

#define APRILPARTICLE_VALUE_SEPARATOR ","

namespace aprilparticle
{
    aprilparticleFnExport void init();
    aprilparticleFnExport void destroy();
	aprilparticleFnExport void setLogFunction(void (*fnptr)(chstr));
	aprilparticleFnExport void log(chstr message, chstr prefix = "[aprilparticle] ");
	aprilparticleFnExport hstr gvec2_to_str(gvec2 vector);
	aprilparticleFnExport hstr gvec3_to_str(gvec3 vector);
	aprilparticleFnExport gvec2 str_to_gvec2(chstr string);
	aprilparticleFnExport gvec3 str_to_gvec3(chstr string);

};

#endif

