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
#include <hltypes/hdir.h>
#include <hltypes/hstring.h>

#include "apriluiparticle.h"
#include "apriluiparticleUtil.h"
#include "ParticleSpace.h"
#include "ParticleBase.h"

namespace apriluiparticle
{
	ParticleBase::ParticleBase(chstr name, grect rect) : aprilui::Object(name, rect)
	{
		this->system = NULL;
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

	void ParticleBase::load(chstr filename)
	{
		this->filename = filename;
		this->stopSystem();
		if ((this->filename != "" || this->filepath != "") && apriluiparticle::isEnabled())
		{
			this->_load();
		}
	}

	void ParticleBase::notifyEvent(chstr name, void* params)
	{	
		if (name == "SettingsChanged")
		{
			if ((this->filename != "" || this->filepath != "") && apriluiparticle::isEnabled())
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
	
	hstr ParticleBase::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "filename")	return this->getFilename();
		if (name == "filepath")	return this->getFilepath();
		return aprilui::Object::getProperty(name, property_exists);
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
		else return aprilui::Object::setProperty(name, value);
		return true;
	}

}
