/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <aprilparticle/System.h>
#include <aprilui/aprilui.h>
#include <gtypes/Rectangle.h>
#include <gtypes/Vector2.h>
#include <hltypes/hstring.h>

#include "apriluiparticle.h"
#include "Particle.h"

namespace apriluiparticle
{
	Particle::Particle(chstr name, grect rect) : ParticleBase(name, rect)
	{
	}
	
	Particle::~Particle()
	{
	}

	aprilui::Object* Particle::createInstance(chstr name, grect rect)
	{
		return new Particle(name, rect);
	}

	void Particle::update(float k)
	{
		if (this->system != NULL)
		{
			this->system->setEnabled(this->isDerivedEnabled());
			this->system->update(k);
		}
		ParticleBase::update(k);
	}

	void Particle::OnDraw()
	{
		grect rect = this->_getDrawRect();
		if (this->system != NULL)
		{
			this->system->setVisible(this->isVisible());
			this->system->draw(rect.getCenter(), this->_getDrawColor());
			april::rendersys->setTextureBlendMode(april::BM_DEFAULT);
		}
		ParticleBase::OnDraw();
		if (aprilui::isDebugEnabled())
		{
			april::rendersys->drawFilledRect(rect, april::Color(april::Color::Yellow, 32));
			april::rendersys->drawRect(rect, april::Color(april::Color::Black, 64));
		}
	}

}
