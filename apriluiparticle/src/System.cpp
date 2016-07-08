/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <aprilparticle/System.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "apriluiparticle.h"
#include "Event.h"
#include "Space.h"
#include "System.h"

namespace apriluiparticle
{
	System::System(chstr name) : Base(name)
	{
	}
	
	System::System(const System& other) : Base(other)
	{
	}

	System::~System()
	{
		foreach (apriluiparticle::Space*, it, this->spaceObjects)
		{
			(*it)->_unbind();
			(*it)->systemObject = NULL;
		}
	}

	aprilui::Object* System::createInstance(chstr name)
	{
		return new System(name);
	}

	harray<Emitter*> System::getEmitterObjects() const
	{
		harray<Emitter*> result;
		foreachc (Space*, it, this->spaceObjects)
		{
			result += (*it)->getEmitterObjects();
		}
		return result;
	}

	void System::_registerSpaceObject(Space* spaceObject)
	{
		this->spaceObjects += spaceObject;
	}

	void System::_unregisterSpaceObject(Space* spaceObject)
	{
		this->spaceObjects -= spaceObject;
	}

	void System::stopSystem()
	{
		if (this->system != NULL)
		{
			foreach (Space*, it, this->spaceObjects)
			{
				(*it)->_unbind();
			}
		}
		Base::stopSystem();
		this->previousExpired = true;
	}

	void System::resetSystem()
	{
		if (this->system != NULL)
		{
			foreach (Space*, it, this->spaceObjects)
			{
				(*it)->_unbind();
			}
		}
		Base::resetSystem();
		this->previousExpired = false;
	}
	
	void System::_load()
	{
		Base::_load();
		this->previousExpired = (this->system == NULL);
	}

	void System::_update(float timeDelta)
	{
		if (this->system != NULL)
		{
			bool expired = this->system->isExpired();
			if (!this->previousExpired && expired)
			{
				this->triggerEvent(Event::ParticleSystemExpired, NULL);
			}
			this->previousExpired = expired;
		}
		else
		{
			this->previousExpired = true;
		}
		Base::_update(timeDelta);
	}

}
