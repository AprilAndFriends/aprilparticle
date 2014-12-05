/// @file
/// @version 2.2
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
	Particle::Particle(chstr name) : Base(name)
	{
		this->debugColor = april::Color(april::Color::LightPurple, 32);
	}

	Particle::Particle(const Particle& other) : Base(other)
	{
	}

	Particle::~Particle()
	{
	}

	aprilui::Object* Particle::createInstance(chstr name)
	{
		return new Particle(name);
	}

	void Particle::update(float timeDelta)
	{
		if (this->system != NULL)
		{
			this->system->setEnabled(this->isDerivedEnabled());
			this->system->update(timeDelta);
		}
		Base::update(timeDelta);
	}

	void Particle::_draw()
	{
		grect rect = this->_getDrawRect();
		if (this->system != NULL)
		{
			this->system->setVisible(this->isDerivedVisible());
			this->system->draw(rect.getCenter(), this->_getDrawColor());
			april::rendersys->setTextureBlendMode(april::BM_DEFAULT);
		}
		Base::_draw();
	}

}
