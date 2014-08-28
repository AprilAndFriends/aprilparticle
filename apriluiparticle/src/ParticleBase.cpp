/// @file
/// @version 2.12
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
#include <hltypes/hdir.h>
#include <hltypes/hstring.h>

#include "apriluiparticle.h"
#include "apriluiparticleUtil.h"
#include "ParticleSpace.h"
#include "ParticleBase.h"

namespace apriluiparticle
{
	harray<aprilui::PropertyDescription> ParticleBase::_propertyDescriptions;

	ParticleBase::ParticleBase(chstr name, grect rect) : aprilui::Object(name, rect)
	{
		this->system = NULL;
		this->alwaysEnabled = false;
	}
	
	ParticleBase::~ParticleBase()
	{
		this->stopSystem();
	}

	aprilui::Object* ParticleBase::createInstance(chstr name, grect rect)
	{
		return new ParticleBase(name, rect);
	}

	bool ParticleBase::isRunning()
	{
		return (this->system != NULL && this->system->isRunning());
	}

	bool ParticleBase::isExpired()
	{
		return (this->system != NULL && this->system->isExpired());
	}

	harray<aprilui::PropertyDescription> ParticleBase::getPropertyDescriptions()
	{
		if (ParticleBase::_propertyDescriptions.size() == 0)
		{
			ParticleBase::_propertyDescriptions += aprilui::PropertyDescription("filename", aprilui::PropertyDescription::STRING);
			ParticleBase::_propertyDescriptions += aprilui::PropertyDescription("filepath", aprilui::PropertyDescription::STRING);
			ParticleBase::_propertyDescriptions += aprilui::PropertyDescription("always_enabled", aprilui::PropertyDescription::BOOL);
		}
		return (aprilui::Object::getPropertyDescriptions() + ParticleBase::_propertyDescriptions);
	}

	void ParticleBase::load(chstr filename)
	{
		this->filename = filename;
		this->stopSystem();
		if ((this->filename != "" || this->filepath != "") && (this->alwaysEnabled || apriluiparticle::isEnabled()))
		{
			this->_load();
		}
	}

	void ParticleBase::notifyEvent(aprilui::Event::Type type, aprilui::EventArgs* args)
	{
		if (type == aprilui::Event::RESIZED)
		{
			this->_resize();
		}
		aprilui::Object::notifyEvent(type, args);
	}

	void ParticleBase::notifyEvent(chstr customType, aprilui::EventArgs* args)
	{
		if (customType == "SettingsChanged")
		{
			if ((this->filename != "" || this->filepath != "") && (this->alwaysEnabled || apriluiparticle::isEnabled()))
			{
				this->_load();
			}
			else
			{
				this->stopSystem();
			}
		}
		aprilui::Object::notifyEvent(customType, args);
	}

	void ParticleBase::_load()
	{
		if (this->system != NULL)
		{
			return;
		}
		hstr filepath = this->filepath;
		if (filepath == "") // if no forced file path exists
		{
			filepath = this->filename;
			hstr defaultPath = apriluiparticle::getDefaultPath();
			if (defaultPath != "")
			{
				filepath = hdir::join_path(defaultPath, filepath, false);
			}
		}
		hstr datasetPath = this->getDataset()->getFilePath();
		if (datasetPath != "")
		{
			filepath = hdir::join_path(datasetPath, filepath, false);
		}
		filepath = hdir::normalize(filepath);
		this->system = aprilparticle::loadSystem(filepath);
		this->_resize();
	}

	void ParticleBase::_resize()
	{
		if (this->system != NULL)
		{
			apriluiparticle::resizeEmitters(this->getSize(), this->system->getEmitters());
		}
	}

	void ParticleBase::finishSystem()
	{
		if (this->system != NULL)
		{
			this->system->finish();
		}
	}

	void ParticleBase::stopSystem()
	{
		if (this->system != NULL)
		{
			delete this->system;
			this->system = NULL;
		}
	}
	
	void ParticleBase::resetSystem()
	{
		if (this->system != NULL)
		{
			this->system->reset();
		}
	}
	
	hstr ParticleBase::getProperty(chstr name)
	{
		if (name == "filename")			return this->getFilename();
		if (name == "filepath")			return this->getFilepath();
		if (name == "always_enabled")	return this->isAlwaysEnabled();
		return aprilui::Object::getProperty(name);
	}

	bool ParticleBase::setProperty(chstr name, chstr value)
	{
		if		(name == "filename")
		{
			this->setFilename(value);
			this->setFilepath("");
			this->notifyEvent("SettingsChanged", NULL);
		}
		else if	(name == "filepath")
		{
			this->setFilepath(value);
			this->setFilename("");
			this->notifyEvent("SettingsChanged", NULL);
		}
		else if	(name == "always_enabled")	this->setAlwaysEnabled(value);
		else return aprilui::Object::setProperty(name, value);
		return true;
	}

}
