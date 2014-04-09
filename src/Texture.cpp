/// @file
/// @author  Boris Mikic
/// @version 2.1
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <april/Texture.h>
#include <hltypes/hstring.h>

#include "aprilparticle.h"
#include "Texture.h"

namespace aprilparticle
{
	Texture::Texture(april::Texture* texture, chstr filename, bool cached)
	{
		this->texture = texture;
		this->filename = filename;
		this->cached = cached;
	}

	Texture::~Texture()
	{
		delete this->texture;
	}
	
}
