/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <april/RenderSystem.h>
#include <aprilparticle/System.h>
#include <aprilui/Dataset.h>
#include <gtypes/Rectangle.h>
#include <hltypes/hlog.h>
#include <hltypes/hstring.h>

#include "apriluiparticle.h"
#include "ParticleEmitter.h"
#include "ParticleAffector.h"

namespace apriluiparticle
{
	ParticleAffector::ParticleAffector(chstr name, grect rect) : aprilui::Object(name, rect)
	{
		this->emitter = NULL;
	}
	
	ParticleAffector::~ParticleAffector()
	{
	}

	aprilui::Object* ParticleAffector::createInstance(chstr name, grect rect)
	{
		return new ParticleAffector(name, rect);
	}

	void ParticleAffector::update(float k)
	{
		if (this->emitter == NULL && this->emitterName != "")
		{
			this->emitter = dynamic_cast<ParticleEmitter*>(this->mDataset->tryGetObject(this->emitterName));
			if (this->emitter == NULL)
			{
				hlog::warnf(apriluiparticle::logTag, "ParticleAffector '%s': referenced emitter '%s' not a subclass of ParticleEmitter!", this->emitterName.c_str(), this->mName.c_str());
				this->emitterName = "";
			}
		}
		aprilui::Object::update(k);
	}
	
	void ParticleAffector::OnDraw()
	{
		aprilui::Object::OnDraw();
	}

	void ParticleAffector::notifyEvent(chstr name, void* params)
	{
		aprilui::Object::notifyEvent(name, params);
	}

	hstr ParticleAffector::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "emitter")	return this->getEmitterName();
		return aprilui::Object::getProperty(name, property_exists);
	}

	bool ParticleAffector::setProperty(chstr name, chstr value)
	{
		if (name == "emitter")	this->setEmitterName(value);
		else return aprilui::Object::setProperty(name, value);
		return true;
	}

}
