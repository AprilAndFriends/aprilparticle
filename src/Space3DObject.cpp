/// @file
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Vector3.h>

#include "Space3DObject.h"

namespace aprilparticle
{
	Space3DObject::Space3DObject(gvec3 position, gvec3 direction)
	{
		this->position = position;
		this->direction = direction;
	}
	
	Space3DObject::~Space3DObject()
	{
	}

}


