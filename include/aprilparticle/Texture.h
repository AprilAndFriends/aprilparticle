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
		Texture(april::Texture* texture, chstr filename, bool cached, bool cachedDefinition);
		Texture(const Texture& other);
		~Texture();

		HL_DEFINE_GET(april::Texture*, texture, Texture);
		HL_DEFINE_GET(hstr, filename, Filename);
		HL_DEFINE_IS(cached, Cached);
		HL_DEFINE_IS(cachedDefinition, CachedDefinition);
		
	protected:
		april::Texture* texture;
		hstr filename;
		bool cached;
		bool cachedDefinition;
		
	};
}

#endif
