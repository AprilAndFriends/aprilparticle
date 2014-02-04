/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <aprilparticle/aprilparticle.h>
#include <aprilparticle/Emitter.h>
#include <aprilparticle/System.h>
#include <aprilui/aprilui.h>
#include <aprilui/Dataset.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "apriluiparticle.h"
#include "apriluiparticleUtil.h"
#include "ParticleEmitter.h"
#include "ParticleSpace.h"
#include "ParticleSystem.h"

namespace apriluiparticle
{
	ParticleEmitter::ParticleEmitter(chstr name, grect rect) : aprilui::Object(name, rect)
	{
		this->spaceObject = NULL;
		this->emitter = NULL;
		this->initialAngle = 0.0f;
	}
	
	ParticleEmitter::~ParticleEmitter()
	{
		if (this->spaceObject != NULL)
		{
			this->spaceObject->_unregisterEmitterObject(this);
		}
	}

	aprilui::Object* ParticleEmitter::createInstance(chstr name, grect rect)
	{
		return new ParticleEmitter(name, rect);
	}

	void ParticleEmitter::update(float k)
	{
		this->_tryFindSpaceObject();
		this->_updateBindings();
		if (this->emitter != NULL)
		{
			this->emitter->setEnabled(this->isDerivedEnabled());
		}
		aprilui::Object::update(k);
	}

	void ParticleEmitter::OnDraw()
	{
		aprilui::Object::OnDraw();
		if (this->spaceObject != NULL && this->emitter != NULL)
		{
			this->_updateEmitterData();
		}
		if (aprilui::isDebugEnabled())
		{
			grect rect = this->_getDrawRect();
			april::rendersys->drawFilledRect(rect, april::Color(april::Color::Neon, 32));
			april::rendersys->drawRect(rect, april::Color(april::Color::Black, 64));
		}
	}

	void ParticleEmitter::_updateEmitterData()
	{
		gvec2 newPosition = this->spaceObject->transformToLocalSpace(this->getDerivedCenter()) - this->initialPosition - this->spaceObject->_getDrawRect().getSize() * 0.5f;
		this->emitter->setPosition(this->emitterPosition + gvec3(newPosition.x, newPosition.y, 0.0f));
		float angle = this->initialAngle - this->_getDerivedAngle();
		gvec2 newMinDirection = gvec2(this->emitterMinDirection.x, this->emitterMinDirection.y).rotated(angle);
		gvec2 newMaxDirection = gvec2(this->emitterMaxDirection.x, this->emitterMaxDirection.y).rotated(angle);
		this->emitter->setMinDirection(gvec3(newMinDirection.x, newMinDirection.y, this->emitterMinDirection.z));
		this->emitter->setMaxDirection(gvec3(newMaxDirection.x, newMaxDirection.y, this->emitterMaxDirection.z));
		this->emitter->setMinDirection(gvec3(newMinDirection.x, newMinDirection.y, this->emitterMinDirection.z));
		this->emitter->setMaxDirection(gvec3(newMaxDirection.x, newMaxDirection.y, this->emitterMaxDirection.z));
		gvec2 scale = this->getDerivedScale() / this->spaceObject->getDerivedScale();
		this->emitter->setMinSize(this->emitterMinSize * scale);
		this->emitter->setMinSize(this->emitterMaxSize * scale);
	}

	void ParticleEmitter::_updateBindings()
	{
		this->_tryFindEmitter();
	}

	void ParticleEmitter::_tryFindSpaceObject()
	{
		if (this->dataset == NULL)
		{
			this->spaceObject = NULL;
			return;
		}
		if (this->spaceObject != NULL && this->spaceObject->getName() == this->spaceObjectName)
		{
			return;
		}
		if (this->spaceObject != NULL)
		{
			this->spaceObject->_unregisterEmitterObject(this);
		}
		this->spaceObject = NULL;
		this->emitter = NULL;
		if (this->spaceObjectName == "")
		{
			return;
		}
		this->spaceObject = dynamic_cast<ParticleSpace*>(this->dataset->tryGetObject(this->spaceObjectName));
		if (this->spaceObject != NULL)
		{
			this->spaceObject->_registerEmitterObject(this);
		}
		else
		{
			hlog::warnf(apriluiparticle::logTag, "ParticleEmitter '%s': referenced object '%s' not a subclass of ParticleSpace!",
				this->spaceObjectName.c_str(), this->name.c_str());
			this->spaceObjectName = "";
			this->emitterName = "";
		}
	}

	void ParticleEmitter::_tryFindEmitter()
	{
		if (this->spaceObject == NULL)
		{
			this->emitter = NULL;
			return;
		}
		if (this->emitter != NULL && this->emitter->getName() == this->emitterName)
		{
			return;
		}
		this->emitter = NULL;
		if (this->emitterName == "")
		{
			return;
		}
		ParticleSystem* systemObject = this->spaceObject->systemObject;
		if (systemObject == NULL)
		{
			return;
		}
		aprilparticle::System* system = systemObject->getSystem();
		if (system == NULL)
		{
			return;
		}
		this->emitter = system->getEmitter(this->emitterName);
		if (this->emitter != NULL)
		{
			this->initialPosition = this->spaceObject->transformToLocalSpace(this->getDerivedCenter());
			this->emitterPosition.set(this->initialPosition.x, this->initialPosition.y, 0.0f);
			this->emitter->setPosition(this->emitterPosition);
			apriluiparticle::resizeEmitter(this->getSize(), this->emitter);
			this->initialAngle = this->_getDerivedAngle();
			this->emitterPosition = this->emitter->getPosition();
			this->emitterMinDirection = this->emitter->getMinDirection();
			this->emitterMaxDirection = this->emitter->getMaxDirection();
			this->emitterMinSize = this->emitter->getMinSize();
			this->emitterMaxSize = this->emitter->getMaxSize();
			this->_updateEmitterData();
		}
		else
		{
			hlog::warnf(apriluiparticle::logTag, "ParticleEmitter '%s': cannot find emitter '%s' in ParticleSpace '%s'!",
				this->name.c_str(), this->emitterName.c_str(), this->spaceObject->getName().c_str());
			this->spaceObjectName = "";
			this->emitterName = "";
		}
	}
	
	void ParticleEmitter::notifyEvent(chstr name, void* params)
	{
		if (name == "OnEnableChanged" && this->emitter != NULL)
		{
			this->emitter->setEnabled(this->isDerivedEnabled());
		}
		Object::notifyEvent(name, params);
	}

	hstr ParticleEmitter::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "space_object")	return this->getSpaceObjectName();
		if (name == "emitter")		return this->getEmitterName();
		return aprilui::Object::getProperty(name, property_exists);
	}

	bool ParticleEmitter::setProperty(chstr name, chstr value)
	{
		if (name == "space_object")	this->setSpaceObjectName(value);
		else if (name == "emitter")	this->setEmitterName(value);
		else return aprilui::Object::setProperty(name, value);
		return true;
	}
	
	void ParticleEmitter::_unbind()
	{
		this->emitter = NULL;
	}

}
