/// @file
/// @author  Boris Mikic
/// @version 2.01
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "apriluiparticle.h"
#include "ParticleSpace.h"
#include "ParticleSystem.h"

namespace apriluiparticle
{
	ParticleSystem::ParticleSystem(chstr name, grect rect) : ParticleBase(name, rect)
	{
	}
	
	ParticleSystem::~ParticleSystem()
	{
		foreach (apriluiparticle::ParticleSpace*, it, this->spaceObjects)
		{
			(*it)->_unbind();
			(*it)->systemObject = NULL;
		}
	}

	aprilui::Object* ParticleSystem::createInstance(chstr name, grect rect)
	{
		return new ParticleSystem(name, rect);
	}

	void ParticleSystem::_registerSpaceObject(ParticleSpace* spaceObject)
	{
		this->spaceObjects += spaceObject;
	}

	void ParticleSystem::_unregisterSpaceObject(ParticleSpace* spaceObject)
	{
		this->spaceObjects -= spaceObject;
	}

	void ParticleSystem::stopSystem()
	{
		if (this->system != NULL)
		{
			foreach (ParticleSpace*, it, this->spaceObjects)
			{
				(*it)->_unbind();
			}
		}
		ParticleBase::stopSystem();
	}

	void ParticleSystem::resetSystem()
	{
		if (this->system != NULL)
		{
			foreach (ParticleSpace*, it, this->spaceObjects)
			{
				(*it)->_unbind();
			}
		}
		ParticleBase::resetSystem();
	}
	
}
