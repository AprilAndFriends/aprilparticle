/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause
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
	void resizeEmitter(cgvec2f size, aprilparticle::Emitter* emitter);
	void resizeEmitters(cgvec2f size, const harray<aprilparticle::Emitter*>& emitters);
	
}
#endif
