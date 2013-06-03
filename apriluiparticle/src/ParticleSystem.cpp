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
#include "ParticleSpace.h"
#include "ParticleSystem.h"

namespace apriluiparticle
{
	ParticleSystem::ParticleSystem(chstr name, grect rect) : aprilui::Object(name, rect)
	{
		this->system = NULL;
	}
	
	ParticleSystem::~ParticleSystem()
	{
		this->stopSystem();
	}

	aprilui::Object* ParticleSystem::createInstance(chstr name, grect rect)
	{
		return new ParticleSystem(name, rect);
	}

	bool ParticleSystem::isRunning()
	{
		return (this->system != NULL && this->system->isRunning());
	}

	bool ParticleSystem::isExpired()
	{
		return (this->system != NULL && this->system->isExpired());
	}

	void ParticleSystem::load(chstr filename)
	{
		this->filename = filename;
		this->stopSystem();
		if (this->filename != "" && apriluiparticle::isEnabled())
		{
			this->_load();
		}
	}

	void ParticleSystem::OnDraw()
	{
		grect rect = this->_getDrawRect();
		aprilui::Object::OnDraw();
		if (aprilui::isDebugEnabled())
		{
			april::rendersys->drawFilledRect(rect, april::Color(april::Color::Yellow, 32));
			april::rendersys->drawRect(rect, april::Color(april::Color::Black, 64));
		}
	}

	void ParticleSystem::notifyEvent(chstr name, void* params)
	{	
		if (name == "SettingsChanged")
		{
			if (this->filename != "" && apriluiparticle::isEnabled())
			{
				this->_load();
			}
			else
			{
				this->stopSystem();
			}
		}
		else if (name == "Resized")
		{
			this->_resize();
		}
		aprilui::Object::notifyEvent(name, params);
	}

	void ParticleSystem::_load()
	{
		if (this->system != NULL)
		{
			return;
		}
		this->_destroySpaceObjects();
		hstr filepath = this->filename;
		hstr defaultPath = apriluiparticle::getDefaultPath();
		if (defaultPath != "")
		{
			filepath = defaultPath + "/" + filepath;
		}
		hstr datasetPath = this->getDataset()->getFilePath();
		if (datasetPath != "")
		{
			filepath = datasetPath + "/" + filepath;
		}
		filepath = normalize_path(filepath);
		this->system = aprilparticle::loadSystem(filepath);
		harray<aprilparticle::Space*> spaces = this->system->getSpaces();
		foreach (aprilparticle::Space*, it, spaces)
		{
			this->_createSpaceObject(*it);
		}
		this->_resize();
	}

	void ParticleSystem::_createSpaceObject(aprilparticle::Space* space)
	{
		ParticleSpaceBase* spaceObject = new ParticleSpaceBase(apriluiparticle::generateName("apriluiparticle::ParticleSpaceBase"),
			grect(0.0f, 0.0f, this->mRect.getSize()));
		spaceObject->_setSystemSpace(this, space);
		this->registerChild(spaceObject);
		this->spaceBaseObjects += spaceObject;
	}

	void ParticleSystem::_destroySpaceObjects()
	{
		foreach (apriluiparticle::ParticleSpaceBase*, it, this->spaceBaseObjects)
		{
			(*it)->destroyChildren();
			this->unregisterChild(*it);
			delete (*it);
		}
		this->spaceBaseObjects.clear();
	}

	void ParticleSystem::_registerSpaceObject(ParticleSpace* spaceObject)
	{
		this->spaceObjects += spaceObject;
	}

	void ParticleSystem::_unregisterSpaceObject(ParticleSpace* spaceObject)
	{
		this->spaceObjects -= spaceObject;
	}

	void ParticleSystem::_assignSpaceObjectSpace(ParticleSpace* spaceObject, aprilparticle::Space* space)
	{
		foreach (apriluiparticle::ParticleSpaceBase*, it, this->spaceBaseObjects)
		{
			if ((*it)->space == space)
			{
				spaceObject->mChildren = (*it)->mChildren;
				(*it)->mChildren.clear();
				this->unregisterChild(*it);
				delete (*it);
				this->spaceBaseObjects += spaceObject;
				return;
			}
		}
	}

	void ParticleSystem::_unassignSpaceObjectSpace(ParticleSpace* spaceObject, aprilparticle::Space* space)
	{
		this->spaceBaseObjects -= spaceObject;
		this->_createSpaceObject(space);
		this->spaceBaseObjects.last()->mChildren = spaceObject->mChildren;
	}

	void ParticleSystem::_resize()
	{
		if (this->system != NULL)
		{
			apriluiparticle::resizeEmitters(this->getSize(), this->system->getEmitters());
		}
	}

	void ParticleSystem::finishSystem()
	{
		if (this->system != NULL)
		{
			this->system->finish();
		}
	}

	void ParticleSystem::stopSystem()
	{
		if (this->system != NULL)
		{
			delete this->system;
			this->system = NULL;
		}
	}
	
	void ParticleSystem::resetSystem()
	{
		if (this->system != NULL)
		{
			this->system->reset();
		}
	}
	
	hstr ParticleSystem::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "filename")		return this->getFilename();
		return aprilui::Object::getProperty(name, property_exists);
	}

	bool ParticleSystem::setProperty(chstr name, chstr value)
	{
		if		(name == "filename")
		{
			this->setFilename(value);
			this->notifyEvent("SettingsChanged", NULL);
		}
		else return aprilui::Object::setProperty(name, value);
		return true;
	}

}
