/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Color.h>
#include <aprilparticle/aprilparticle.h>
#include <aprilparticle/Emitter.h>
#include <aprilparticle/Space.h>
#include <aprilparticle/System.h>
#include <aprilui/aprilui.h>
#include <aprilui/Dataset.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/hstring.h>

#include "apriluiparticle.h"
#include "apriluiparticleUtil.h"
#include "ParticleEmitter.h"
#include "ParticleSpace.h"
#include "ParticleSystem.h"

namespace apriluiparticle
{
	ParticleSpace::ParticleSpace(chstr name, grect rect) : ParticleSpaceBase(name, rect)
	{
	}
	
	ParticleSpace::~ParticleSpace()
	{
		foreach (ParticleEmitter*, it, this->emitterObjects)
		{
			(*it)->_unbind();
		}
		if (this->systemObject != NULL)
		{
			if (this->space != NULL)
			{
				this->systemObject->_unassignSpaceObjectSpace(this, this->space);
			}
			this->systemObject->_unregisterSpaceObject(this);
			this->systemObject = NULL;
			this->space = NULL;
		}
	}

	aprilui::Object* ParticleSpace::createInstance(chstr name, grect rect)
	{
		return new ParticleSpace(name, rect);
	}

	void ParticleSpace::update(float k)
	{
		this->_tryFindSystemObject();
		this->_tryFindSpace();
		ParticleSpaceBase::update(k);
	}

	void ParticleSpace::OnDraw()
	{
		this->_tryFindSystemObject();
		this->_tryFindSpace();
		ParticleSpaceBase::OnDraw();
		if (aprilui::isDebugEnabled())
		{
			grect rect = this->_getDrawRect();
			april::rendersys->drawFilledRect(rect, april::Color(april::Color::Orange, 32));
			april::rendersys->drawRect(rect, april::Color(april::Color::Black, 64));
		}
	}

	void ParticleSpace::notifyEvent(chstr name, void* params)
	{	
		if (name == "Resized")
		{
			this->_resize();
		}
		ParticleSpaceBase::notifyEvent(name, params);
	}

	void ParticleSpace::_tryFindSystemObject()
	{
		if (this->mDataset == NULL)
		{
			return;
		}
		if (this->systemObject != NULL && this->systemObject->getName() == this->systemObjectName)
		{
			return;
		}
		this->_reset();
		if (this->systemObject != NULL)
		{
			if (this->space != NULL)
			{
				this->systemObject->_unassignSpaceObjectSpace(this, this->space);
			}
			this->systemObject->_unregisterSpaceObject(this);
		}
		if (this->systemObjectName != "")
		{
			this->systemObject = dynamic_cast<ParticleSystem*>(this->mDataset->tryGetObject(this->systemObjectName));
			if (this->systemObject != NULL)
			{
				this->systemObject->_registerSpaceObject(this);
			}
			else
			{
				hlog::warnf(apriluiparticle::logTag, "ParticleSpace '%s': referenced object '%s' not a subclass of ParticleSystem!",
					this->systemObjectName.c_str(), this->mName.c_str());
				this->systemObjectName = "";
				this->spaceName = "";
				this->space = NULL;
			}
		}
	}

	void ParticleSpace::_tryFindSpace()
	{
		if (this->systemObject == NULL)
		{
			return;
		}
		aprilparticle::System* system = this->systemObject->getSystem();
		if (system == NULL)
		{
			return;
		}
		if (this->space != NULL && this->space->getName() == this->spaceName)
		{
			return;
		}
		this->_reset();
		if (this->space != NULL)
		{
			this->systemObject->_unassignSpaceObjectSpace(this, this->space);
		}
		if (this->spaceName != "")
		{
			this->space = system->getSpace(this->spaceName);
			if (this->space != NULL)
			{
				this->systemObject->_assignSpaceObjectSpace(this, this->space);
			}
			else
			{
				hlog::warnf(apriluiparticle::logTag, "ParticleSpace '%s': cannot find space '%s' in ParticleSystem '%s'!",
					this->mName.c_str(), this->spaceName.c_str(), this->systemObject->getName().c_str());
				this->systemObject->_unregisterSpaceObject(this);
				this->systemObject = NULL;
				this->systemObjectName = "";
				this->spaceName = "";
			}
		}
	}

	void ParticleSpace::_unbind()
	{
		this->systemObject = NULL;
		this->space = NULL;
		foreach (ParticleEmitter*, it, this->emitterObjects)
		{
			(*it)->emitter = NULL;
		}
	}

	void ParticleSpace::_registerEmitterObject(ParticleEmitter* emitter)
	{
		this->emitterObjects += emitter;
	}

	void ParticleSpace::_unregisterEmitterObject(ParticleEmitter* emitter)
	{
		this->emitterObjects -= emitter;
	}

	void ParticleSpace::_resize()
	{
		if (this->space != NULL)
		{
			apriluiparticle::resizeEmitters(this->getSize(), this->space->getEmitters());
		}
	}

	hstr ParticleSpace::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "system_object")	return this->getSystemObjectName();
		if (name == "space")			return this->getSpaceName();
		return ParticleSpaceBase::getProperty(name, property_exists);
	}

	bool ParticleSpace::setProperty(chstr name, chstr value)
	{
		if (name == "system_object")	this->setSystemObjectName(value);
		else if (name == "space")		this->setSpaceName(value);
		else return ParticleSpaceBase::setProperty(name, value);
		return true;
	}

}
