/// @file
/// @author  Boris Mikic
/// @version 1.5
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Texture.h>
#include <hltypes/hstring.h>

#include "Texture.h"

namespace aprilparticle
{
	Texture::Texture(april::Texture* texture, bool cached)
	{
		this->texture = texture;
		this->cached = cached;
	}
	
	Texture::~Texture()
	{
		delete this->texture;
	}
	
}
