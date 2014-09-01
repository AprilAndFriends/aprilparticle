/// @file
/// @version 2.11
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "apriluiparticle.h"
#include "apriluiparticleUtil.h"
#include "ParticleEmitter.h"
#include "ParticleSpace.h"
#include "ParticleSystem.h"

namespace apriluiparticle
{
	harray<aprilui::PropertyDescription> ParticleSpace::_propertyDescriptions;

	ParticleSpace::ParticleSpace(chstr name, grect rect) : aprilui::Object(name, rect)
	{
		this->systemObject = NULL;
		this->space = NULL;
	}
	
	ParticleSpace::~ParticleSpace()
	{
		if (this->systemObject != NULL)
		{
			this->systemObject->_unregisterSpaceObject(this);
		}
		foreach (apriluiparticle::ParticleEmitter*, it, this->emitterObjects)
		{
			(*it)->_unbind();
			(*it)->spaceObject = NULL;
		}
	}

	aprilui::Object* ParticleSpace::createInstance(chstr name, grect rect)
	{
		return new ParticleSpace(name, rect);
	}

	harray<aprilui::PropertyDescription> ParticleSpace::getPropertyDescriptions()
	{
		if (ParticleSpace::_propertyDescriptions.size() == 0)
		{
			ParticleSpace::_propertyDescriptions += aprilui::PropertyDescription("system_object", aprilui::PropertyDescription::STRING);
			ParticleSpace::_propertyDescriptions += aprilui::PropertyDescription("space", aprilui::PropertyDescription::STRING);
		}
		return (aprilui::Object::getPropertyDescriptions() + ParticleSpace::_propertyDescriptions);
	}

	void ParticleSpace::update(float timeDelta)
	{
		this->_updateBindings();
		foreach (apriluiparticle::ParticleEmitter*, it, this->emitterObjects)
		{
			(*it)->_updateBindings();
		}
		if (this->space != NULL)
		{
			this->space->setEnabled(this->isDerivedEnabled());
			this->space->update(timeDelta);
		}
		aprilui::Object::update(timeDelta);
	}

	void ParticleSpace::OnDraw()
	{
		if (this->space != NULL)
		{
			this->space->setVisible(this->isVisible());
			this->space->draw(this->_getDrawRect().getCenter(), this->_getDrawColor());
			april::rendersys->setTextureBlendMode(april::BM_DEFAULT);
		}
		aprilui::Object::OnDraw();
		if (aprilui::isDebugEnabled())
		{
			grect rect = this->_getDrawRect();
			april::rendersys->drawFilledRect(rect, april::Color(april::Color::Orange, 32));
			april::rendersys->drawRect(rect, april::Color(april::Color::Black, 64));
		}
	}

	void ParticleSpace::_updateBindings()
	{
		this->_tryFindSystemObject();
		this->_tryFindSpace();
	}

	void ParticleSpace::_tryFindSystemObject()
	{
		if (this->dataset == NULL)
		{
			this->systemObject = NULL;
			return;
		}
		if (this->systemObject != NULL && this->systemObject->getName() == this->systemObjectName)
		{
			return;
		}
		this->systemObject = NULL;
		this->space = NULL;
		if (this->systemObjectName == "")
		{
			return;
		}
		this->systemObject = dynamic_cast<ParticleSystem*>(this->dataset->tryGetObject(this->systemObjectName));
		if (this->systemObject != NULL)
		{
			this->systemObject->_registerSpaceObject(this);
		}
		else
		{
			hlog::warnf(apriluiparticle::logTag, "ParticleSpace '%s': referenced object '%s' not a subclass of ParticleSystem!",
				this->systemObjectName.c_str(), this->name.c_str());
			this->systemObjectName = "";
			this->spaceName = "";
		}
	}

	void ParticleSpace::_tryFindSpace()
	{
		if (this->systemObject == NULL)
		{
			this->space = NULL;
			return;
		}
		if (this->space != NULL && this->space->getName() == this->spaceName)
		{
			return;
		}
		this->space = NULL;
		if (this->spaceName == "")
		{
			return;
		}
		aprilparticle::System* system = this->systemObject->getSystem();
		if (system == NULL)
		{
			return;
		}
		this->space = system->getSpace(this->spaceName);
		if (this->space == NULL)
		{
			hlog::warnf(apriluiparticle::logTag, "ParticleSpace '%s': cannot find space '%s' in ParticleSystem '%s'!",
				this->name.c_str(), this->spaceName.c_str(), this->systemObject->getName().c_str());
			this->spaceName = "";
		}
	}

	void ParticleSpace::notifyEvent(chstr type, aprilui::EventArgs* args)
	{	
		if (type == aprilui::Event::Resized)
		{
			this->_resize();
		}
		aprilui::Object::notifyEvent(type, args);
	}

	void ParticleSpace::_registerEmitterObject(ParticleEmitter* emitter)
	{
		this->emitterObjects += emitter;
	}

	void ParticleSpace::_unregisterEmitterObject(ParticleEmitter* emitter)
	{
		this->emitterObjects -= emitter;
	}

	void ParticleSpace::_unbind()
	{
		this->space = NULL;
		foreach (apriluiparticle::ParticleEmitter*, it, this->emitterObjects)
		{
			(*it)->_unbind();
		}
	}
	
	void ParticleSpace::_resize()
	{
		if (this->space != NULL)
		{
			apriluiparticle::resizeEmitters(this->getSize(), this->space->getEmitters());
		}
	}

	hstr ParticleSpace::getProperty(chstr name)
	{
		if (name == "system_object")	return this->getSystemObjectName();
		if (name == "space")			return this->getSpaceName();
		return aprilui::Object::getProperty(name);
	}

	bool ParticleSpace::setProperty(chstr name, chstr value)
	{
		if (name == "system_object")	this->setSystemObjectName(value);
		else if (name == "space")		this->setSpaceName(value);
		else return aprilui::Object::setProperty(name, value);
		return true;
	}

}
