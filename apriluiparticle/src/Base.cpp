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
	hmap<hstr, aprilui::PropertyDescription> Base::_propertyDescriptions;
	hmap<hstr, aprilui::PropertyDescription::Accessor*> Base::_getters;
	hmap<hstr, aprilui::PropertyDescription::Accessor*> Base::_setters;

	Base::Base(chstr name) : aprilui::Object(name)
	{
		this->loaded = false;
		this->system = NULL;
		this->alwaysEnabled = false;
	}
	
	Base::Base(const Base& other) : aprilui::Object(other)
	{
		this->filename = other.filename;
		this->filepath = other.filepath;
		this->loaded = other.loaded;
		this->system = NULL;
		this->alwaysEnabled = other.alwaysEnabled;
		if (other.system != NULL)
		{
			this->system = new aprilparticle::System(*other.system); // make a clone
		}
	}

	Base::~Base()
	{
		// must not call stopSystem(), because it's virtual
		if (this->system != NULL)
		{
			delete this->system;
			this->system = NULL;
		}
	}

	hmap<hstr, aprilui::PropertyDescription>& Base::getPropertyDescriptions() const
	{
		if (Base::_propertyDescriptions.size() == 0)
		{
			Base::_propertyDescriptions = aprilui::Object::getPropertyDescriptions();
			Base::_propertyDescriptions["filename"] = aprilui::PropertyDescription("filename", aprilui::PropertyDescription::Type::String);
			Base::_propertyDescriptions["filepath"] = aprilui::PropertyDescription("filepath", aprilui::PropertyDescription::Type::String);
			Base::_propertyDescriptions["always_enabled"] = aprilui::PropertyDescription("always_enabled", aprilui::PropertyDescription::Type::Bool);
		}
		return Base::_propertyDescriptions;
	}

	hmap<hstr, aprilui::PropertyDescription::Accessor*>& Base::_getGetters() const
	{
		if (Base::_getters.size() == 0)
		{
			Base::_getters = aprilui::Object::_getGetters();
			Base::_getters["filename"] = new aprilui::PropertyDescription::Get<Base, hstr>(&Base::getFilename);
			Base::_getters["filepath"] = new aprilui::PropertyDescription::Get<Base, hstr>(&Base::getFilepath);
			Base::_getters["always_enabled"] = new aprilui::PropertyDescription::Get<Base, bool>(&Base::isAlwaysEnabled);
		}
		return Base::_getters;
	}

	hmap<hstr, aprilui::PropertyDescription::Accessor*>& Base::_getSetters() const
	{
		if (Base::_setters.size() == 0)
		{
			Base::_setters = aprilui::Object::_getSetters();
			Base::_setters["always_enabled"] = new aprilui::PropertyDescription::Set<Base, bool>(&Base::setAlwaysEnabled);
		}
		return Base::_setters;
	}

	bool Base::isRunning() const
	{
		return (this->system != NULL && this->system->isRunning());
	}

	bool Base::isExpired() const
	{
		return (this->system != NULL && this->system->isExpired());
	}

	void Base::notifyEvent(chstr type, aprilui::EventArgs* args)
	{
		if (type == aprilui::Event::Resized)
		{
			this->_resize();
		}
		else if (type == Event::ParticleSettingsChanged)
		{
			this->loaded = false;
			if (this->filename != "" || this->filepath != "")
			{
				this->loaded = true;
				if (this->alwaysEnabled || apriluiparticle::isEnabled())
				{
					this->_load();
				}
				else
				{
					this->stopSystem();
				}
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
		this->loaded = false;
		this->filename = filename;
		this->stopSystem();
		if (this->filename != "" || this->filepath != "")
		{
			this->loaded = true;
			if (this->alwaysEnabled || apriluiparticle::isEnabled())
			{
				this->_load();
			}
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
	
	bool Base::setProperty(chstr name, chstr value)
	{
		if (name == "filename")
		{
			this->setFilename(value);
			this->setFilepath("");
			this->notifyEvent(Event::ParticleSettingsChanged, NULL);
			return true;
		}
		if	(name == "filepath")
		{
			this->setFilepath(value);
			this->setFilename("");
			this->notifyEvent(Event::ParticleSettingsChanged, NULL);
			return true;
		}
		return aprilui::Object::setProperty(name, value);
	}

}
