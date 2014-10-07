/// @file
/// @version 2.11
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

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
		this->debugColor = april::Color(april::Color::LightPurple, 32);
	}
	
	Particle::~Particle()
	{
	}

	aprilui::Object* Particle::createInstance(chstr name, grect rect)
	{
		return new Particle(name, rect);
	}

	void Particle::update(float timeDelta)
	{
		if (this->system != NULL)
		{
			this->system->setEnabled(this->isDerivedEnabled());
			this->system->update(timeDelta);
		}
		ParticleBase::update(timeDelta);
	}

	void Particle::OnDraw()
	{
		grect rect = this->_getDrawRect();
		if (this->system != NULL)
		{
			this->system->setVisible(this->isDerivedVisible());
			this->system->draw(rect.getCenter(), this->_getDrawColor());
			april::rendersys->setTextureBlendMode(april::BM_DEFAULT);
		}
		ParticleBase::OnDraw();
	}

}
