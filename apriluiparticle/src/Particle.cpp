/// @file
/// @version 2.3
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
#include "Event.h"
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

	void Particle::_update(float timeDelta)
	{
		if (this->system != NULL)
		{
			this->system->setEnabled(this->isDerivedEnabled());
			bool expired = this->system->isExpired();
			this->system->update(timeDelta);
			if (!expired && this->system->isExpired())
			{
				this->triggerEvent(Event::ParticleSystemExpired, NULL);
			}
		}
		Base::_update(timeDelta);
	}

	void Particle::_draw()
	{
		grect rect = this->_makeDrawRect();
		if (this->system != NULL)
		{
			this->system->setVisible(this->isDerivedVisible());
			this->system->draw(rect.getCenter(), this->_makeDrawColor());
		}
		Base::_draw();
	}

}
