/// @file
/// @author  Boris Mikic
/// @version 1.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a texture wrapper for april::Texture.

#ifndef APRILPARTICLE_TEXTURE_H
#define APRILPARTICLE_TEXTURE_H

#include <hltypes/hstring.h>

#include "aprilparticleExport.h"

namespace april
{
	class Texture;
}

namespace aprilparticle
{
	class aprilparticleExport Texture
	{
	public:
		Texture(april::Texture* texture, bool cached);
		~Texture();

		HL_DEFINE_GET(april::Texture*, texture, Texture);
		HL_DEFINE_IS(bool, cached, Cached);
		
	protected:
		april::Texture* texture;
		bool cached;
		
	};
}

#endif
