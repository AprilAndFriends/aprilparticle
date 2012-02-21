/// @file
/// @author  Boris Mikic
/// @version 1.4
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/Color.h>
#include <aprilparticle/Emitter.h>
#include <aprilparticle/System.h>
#include <aprilui/aprilui.h>
#include <aprilui/Dataset.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <gtypes/Vector3.h>
#include <hltypes/hstring.h>

#include "apriluiparticle.h"
#include "Particle.h"

namespace apriluiparticle
{
	Particle::Particle(chstr name, grect rect) : aprilui::Object(name, rect)
	{
		this->globalSpace = false;
		this->initialPosition = this->getPosition();
		this->system = NULL;
	}
	
	Particle::~Particle()
	{
		this->stopSystem();
	}

	aprilui::Object* Particle::createInstance(chstr name, grect rect)
	{
		return new Particle(name, rect);
	}

	bool Particle::isRunning()
	{
		return (this->system != NULL && this->system->isRunning());
	}

	bool Particle::isExpired()
	{
		return (this->system != NULL && this->system->isExpired());
	}

	void Particle::load(chstr filename)
	{
		this->filename = filename;
		this->stopSystem();
		this->_loadParticleSystem();
	}

	void Particle::update(float k)
	{
		if (this->system != NULL)
		{
			this->system->setEnabled(this->_isDerivedEnabled());
			this->system->update(k);
		}
		aprilui::Object::update(k);
	}
	
	void Particle::OnDraw()
	{
		if (this->system != NULL)
		{
			this->system->setVisible(this->isVisible());
			if (!this->globalSpace)
			{
				this->system->draw(gvec2(), this->_getDrawColor());
			}
			else
			{
				gvec2 position = this->initialPosition - this->getPosition();
				this->system->setPosition(this->systemPosition.x - position.x, this->systemPosition.y - position.y, this->systemPosition.z);
				this->system->draw(position, this->_getDrawColor());
			}
			april::rendersys->setBlendMode(april::DEFAULT);
		}
		aprilui::Object::OnDraw();
	}

	void Particle::notifyEvent(chstr name, void* params)
	{	
		if (name == "SettingsChanged")
		{
			if (this->filename != "" && apriluiparticle::isEnabled())
			{
				this->_loadParticleSystem();
			}
			else
			{
				this->stopSystem();
			}
		}
		aprilui::Object::notifyEvent(name, params);
	}

	void Particle::_loadParticleSystem()
	{
		if (this->system != NULL)
		{
			return;
		}
		hstr filepath = this->filename;
		hstr defaultPath = apriluiparticle::getDefaultPath();
		if (defaultPath != "")
		{
			filepath = defaultPath + "/" + filepath;
		}
		hstr datasetPath = this->getDataset()->_getFilePath();
		if (datasetPath != "")
		{
			filepath = datasetPath + "/" + filepath;
		}
		filepath = normalize_path(filepath);
		this->system = new aprilparticle::System(filepath);
		this->system->load();
		this->systemPosition = this->system->getPosition();
		grect rect = this->getRect();
		if (rect.w > 0.0f || rect.h > 0.0f)
		{
			harray<aprilparticle::Emitter*> emitters = this->system->getEmitters();
			if (emitters.size() > 0)
			{
				gvec2 rescale(1.0f, 1.0f);
				gvec3 dimensions = emitters.first()->getDimensions();
				if (rect.w > 0.0f)
				{
					rescale.x = (dimensions.x > 0.0f ? rect.w / dimensions.x : rect.w);
				}
				if (rect.h > 0.0f)
				{
					rescale.y = (dimensions.y > 0.0f ? rect.h / dimensions.y : rect.h);
				}
				foreach (aprilparticle::Emitter*, it, emitters)
				{
					dimensions = (*it)->getDimensions();
					dimensions.x = (dimensions.x > 0.0f ? dimensions.x * rescale.x : rescale.x);
					dimensions.y = (dimensions.y > 0.0f ? dimensions.y * rescale.y : rescale.y);
					(*it)->setDimensions(dimensions);
				}
			}
		}
	}

	void Particle::finishSystem()
	{
		if (this->system != NULL)
		{
			this->system->finish();
		}
	}

	void Particle::stopSystem()
	{
		if (this->system != NULL)
		{
			delete this->system;
			this->system = NULL;
		}
	}
	
	void Particle::resetSystem()
	{
		if (this->system != NULL)
		{
			this->system->reset();
		}
	}
	
	hstr Particle::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "filename")		return this->getFilename();
		if (name == "global_space")	return this->isGlobalSpace();
		return aprilui::Object::getProperty(name, property_exists);
	}

	bool Particle::setProperty(chstr name, chstr value)
	{
		if		(name == "filename")
		{
			this->setFilename(value);
			this->notifyEvent("SettingsChanged", NULL);
		}
		else if	(name == "global_space")	this->setGlobalSpace(value);
		else return aprilui::Object::setProperty(name, value);
		return true;
	}

}
