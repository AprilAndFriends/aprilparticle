/// @file
/// @version 2.3
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
#include "Emitter.h"
#include "Space.h"
#include "System.h"

namespace apriluiparticle
{
	hmap<hstr, aprilui::PropertyDescription> Emitter::_propertyDescriptions;

	Emitter::Emitter(chstr name) : aprilui::Object(name)
	{
		this->spaceObject = NULL;
		this->emitter = NULL;
		this->initialAngle = 0.0f;
		this->debugColor.set(april::Color::LightNeon, 32);
	}
	
	Emitter::Emitter(const Emitter& other) : aprilui::Object(other)
	{
		this->spaceObject = NULL;
		this->emitter = NULL;
		this->spaceObjectName = other.spaceObjectName;
		this->emitterName = other.emitterName;
		this->initialPosition = other.initialPosition;
		this->initialAngle = other.initialAngle;
		this->emitterPosition = other.emitterPosition;
		this->emitterMinDirection = other.emitterMinDirection;
		this->emitterMaxDirection = other.emitterMaxDirection;
		this->emitterMinSize = other.emitterMinSize;
		this->emitterMaxSize = other.emitterMaxSize;
	}

	Emitter::~Emitter()
	{
		if (this->spaceObject != NULL)
		{
			this->spaceObject->_unregisterEmitterObject(this);
		}
	}

	aprilui::Object* Emitter::createInstance(chstr name)
	{
		return new Emitter(name);
	}

	hmap<hstr, aprilui::PropertyDescription>& Emitter::getPropertyDescriptions() const
	{
		if (Emitter::_propertyDescriptions.size() == 0)
		{
			Emitter::_propertyDescriptions = aprilui::Object::getPropertyDescriptions();
			Emitter::_propertyDescriptions["space_object"] = aprilui::PropertyDescription("space_object", aprilui::PropertyDescription::Type::String);
			Emitter::_propertyDescriptions["emitter"] = aprilui::PropertyDescription("emitter", aprilui::PropertyDescription::Type::String);
		}
		return Emitter::_propertyDescriptions;
	}

	void Emitter::resetEmitter()
	{
		if (this->emitter != NULL)
		{
			this->emitter->reset();
		}
	}

	void Emitter::_update(float timeDelta)
	{
		this->_tryFindSpaceObject();
		this->_updateBindings();
		if (this->emitter != NULL)
		{
			this->emitter->setEnabled(this->isDerivedEnabled());
		}
		aprilui::Object::_update(timeDelta);
	}

	void Emitter::_draw()
	{
		aprilui::Object::_draw();
		if (this->spaceObject != NULL && this->emitter != NULL)
		{
			this->_updateEmitterData();
		}
	}

	void Emitter::_updateEmitterData()
	{
		gvec2 newPosition = this->spaceObject->transformToLocalSpace(this->getDerivedPivot()) - this->initialPosition - this->spaceObject->_makeDrawRect().getSize() * 0.5f;
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

	void Emitter::_updateBindings()
	{
		this->_tryFindEmitter();
	}

	void Emitter::_tryFindSpaceObject()
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
		this->spaceObject = dynamic_cast<Space*>(this->dataset->tryGetObject(this->spaceObjectName));
		if (this->spaceObject != NULL)
		{
			this->spaceObject->_registerEmitterObject(this);
		}
		else
		{
			hlog::warnf(logTag, "Emitter '%s': referenced object '%s' not a subclass of Space!", this->name.cStr(), this->spaceObjectName.cStr());
			this->spaceObjectName = "";
			this->emitterName = "";
		}
	}

	void Emitter::_tryFindEmitter()
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
		System* systemObject = this->spaceObject->systemObject;
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
			this->initialPosition = this->spaceObject->transformToLocalSpace(this->getDerivedPivot());
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
			hlog::warnf(logTag, "Emitter '%s': cannot find emitter '%s' in Space '%s'!", this->name.cStr(), this->emitterName.cStr(), this->spaceObject->getName().cStr());
			this->spaceObjectName = "";
			this->emitterName = "";
		}
	}
	
	void Emitter::notifyEvent(chstr type, aprilui::EventArgs* args)
	{
		if (type == aprilui::Event::EnabledChanged && this->emitter != NULL)
		{
			this->emitter->setEnabled(this->isDerivedEnabled());
		}
		Object::notifyEvent(type, args);
	}

	hstr Emitter::getProperty(chstr name)
	{
		if (name == "space_object")	return this->getSpaceObjectName();
		if (name == "emitter")		return this->getEmitterName();
		return aprilui::Object::getProperty(name);
	}

	bool Emitter::setProperty(chstr name, chstr value)
	{
		if (name == "space_object")	this->setSpaceObjectName(value);
		else if (name == "emitter")	this->setEmitterName(value);
		else return aprilui::Object::setProperty(name, value);
		return true;
	}
	
	void Emitter::_unbind()
	{
		this->emitter = NULL;
	}

}
