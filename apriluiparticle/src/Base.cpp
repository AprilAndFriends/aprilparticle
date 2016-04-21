/// @file
/// @version 2.3
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
#include "Base.h"
#include "Event.h"
#include "Space.h"

namespace apriluiparticle
{
	harray<aprilui::PropertyDescription> Base::_propertyDescriptions;

	Base::Base(chstr name) : aprilui::Object(name)
	{
		this->system = NULL;
		this->alwaysEnabled = false;
	}
	
	Base::Base(const Base& other) : aprilui::Object(other)
	{
		this->filename = other.filename;
		this->filepath = other.filepath;
		this->alwaysEnabled = other.alwaysEnabled;
		this->system = new aprilparticle::System(*other.system); // make a clone
	}

	Base::~Base()
	{
		this->stopSystem();
	}

	bool Base::isRunning()
	{
		return (this->system != NULL && this->system->isRunning());
	}

	bool Base::isExpired()
	{
		return (this->system != NULL && this->system->isExpired());
	}

	harray<aprilui::PropertyDescription> Base::getPropertyDescriptions()
	{
		if (Base::_propertyDescriptions.size() == 0)
		{
			Base::_propertyDescriptions += aprilui::PropertyDescription("filename", aprilui::PropertyDescription::STRING);
			Base::_propertyDescriptions += aprilui::PropertyDescription("filepath", aprilui::PropertyDescription::STRING);
			Base::_propertyDescriptions += aprilui::PropertyDescription("always_enabled", aprilui::PropertyDescription::BOOL);
		}
		return (aprilui::Object::getPropertyDescriptions() + Base::_propertyDescriptions);
	}

	void Base::notifyEvent(chstr type, aprilui::EventArgs* args)
	{
		if (type == aprilui::Event::Resized)
		{
			this->_resize();
		}
		else if (type == Event::ParticleSettingsChanged)
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
		aprilui::Object::notifyEvent(type, args);
	}

	void Base::load(chstr filename)
	{
		this->filename = filename;
		this->stopSystem();
		if ((this->filename != "" || this->filepath != "") && (this->alwaysEnabled || apriluiparticle::isEnabled()))
		{
			this->_load();
		}
	}

	void Base::_load()
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
				filepath = hdir::joinPath(defaultPath, filepath, false);
			}
		}
		hstr datasetPath = this->getDataset()->getFilePath();
		if (datasetPath != "")
		{
			filepath = hdir::joinPath(datasetPath, filepath, false);
		}
		filepath = hdir::normalize(filepath);
		this->system = aprilparticle::loadSystem(filepath);
		this->_resize();
	}

	void Base::_resize()
	{
		if (this->system != NULL)
		{
			apriluiparticle::resizeEmitters(this->getSize(), this->system->getEmitters());
		}
	}

	void Base::finishSystem()
	{
		if (this->system != NULL)
		{
			this->system->finish();
		}
	}

	void Base::stopSystem()
	{
		if (this->system != NULL)
		{
			delete this->system;
			this->system = NULL;
		}
	}
	
	void Base::resetSystem()
	{
		if (this->system != NULL)
		{
			this->system->reset();
		}
	}
	
	hstr Base::getProperty(chstr name)
	{
		if (name == "filename")			return this->getFilename();
		if (name == "filepath")			return this->getFilepath();
		if (name == "always_enabled")	return this->isAlwaysEnabled();
		return aprilui::Object::getProperty(name);
	}

	bool Base::setProperty(chstr name, chstr value)
	{
		if		(name == "filename")
		{
			this->setFilename(value);
			this->setFilepath("");
			this->notifyEvent(Event::ParticleSettingsChanged, NULL);
		}
		else if	(name == "filepath")
		{
			this->setFilepath(value);
			this->setFilename("");
			this->notifyEvent(Event::ParticleSettingsChanged, NULL);
		}
		else if	(name == "always_enabled")	this->setAlwaysEnabled(value);
		else return aprilui::Object::setProperty(name, value);
		return true;
	}

}
