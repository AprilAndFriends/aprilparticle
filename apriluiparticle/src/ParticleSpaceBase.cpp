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
#include "ParticleSpaceBase.h"
#include "ParticleSystem.h"

namespace apriluiparticle
{
	ParticleSpaceBase::ParticleSpaceBase(chstr name, grect rect) : aprilui::Object(name, rect)
	{
		this->systemObject = NULL;
		this->space = NULL;
	}
	
	ParticleSpaceBase::~ParticleSpaceBase()
	{
	}

	aprilui::Object* ParticleSpaceBase::createInstance(chstr name, grect rect)
	{
		return new ParticleSpaceBase(name, rect);
	}

	void ParticleSpaceBase::_setSystemSpace(ParticleSystem* systemObject, aprilparticle::Space* space)
	{
		this->systemObject = systemObject;
		this->space = space;
	}

	void ParticleSpaceBase::update(float k)
	{
		if (this->space != NULL)
		{
			this->space->setEnabled(this->isDerivedEnabled());
			this->space->update(k);
		}
		aprilui::Object::update(k);
	}

	void ParticleSpaceBase::OnDraw()
	{
		if (this->space != NULL)
		{
			this->space->setVisible(this->isVisible());
			this->space->draw(this->_getDrawRect().getCenter(), this->_getDrawColor());
			april::rendersys->setTextureBlendMode(april::DEFAULT);
		}
		aprilui::Object::OnDraw();
	}

}
