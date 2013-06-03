/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Defines utility functions used internally.

#ifndef APRILUIPARTICLE_UTIL_H
#define APRILUIPARTICLE_UTIL_H

#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "apriluiparticleExport.h"

namespace aprilparticle
{
	class Emitter;
}

namespace apriluiparticle
{
	hstr generateName(chstr prefix);
	void resizeEmitter(gvec2 size, aprilparticle::Emitter* emitter);
	void resizeEmitters(gvec2 size, harray<aprilparticle::Emitter*> emitters);
	
}
#endif
