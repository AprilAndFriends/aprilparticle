/// @file
/// @author  Boris Mikic
/// @version 2.11
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include <gtypes/Vector3.h>
#include <hltypes/hdeque.h>
#include <hltypes/hlog.h>
#include <hltypes/hltypesUtil.h>

#include "Affector.h"
#include "aprilparticleUtil.h"
#include "Emitter.h"
#include "Particle.h"
#include "Space.h"

#define PRE_UPDATE_TIME_STEP 0.05f

namespace aprilparticle
{
	Space::Space(chstr name) : SpaceObject(name == "" ? generateName("Space") : name), AffectorContainer(), _lastTimeFraction(0.0f)
	{
		this->preUpdate = 0.0f;
		this->fixedTimeStep = 0.0f;
		this->up.set(0.0f, 1.0f, 0.0f);
		this->alive = 0;
		this->started = false;
		this->system = NULL;
		
	}

	Space::Space(const Space& other) : SpaceObject(other), AffectorContainer(other), _lastTimeFraction(0.0f)
	{
		this->preUpdate = other.preUpdate;
		this->fixedTimeStep = other.fixedTimeStep;
		this->up = other.up;
		this->alive = 0;
		this->started = other.started;
		this->system = NULL;
	}
	
	Space::~Space()
	{
		foreach (Emitter*, it, this->emitters)
		{
			delete (*it);
		}
		foreach_q (Particle*, it, this->particles)
		{
			delete (*it);
		}
	}

	bool Space::isRunning()
	{
		foreach (Emitter*, it, this->emitters)
		{
			if ((*it)->isRunning())
			{
				return true;
			}
		}
		return false;
	}

	bool Space::isExpired()
	{
		foreach (Emitter*, it, this->emitters)
		{
			if (!(*it)->isExpired())
			{
				return false;
			}
		}
		return true;
	}

	int Space::getParticleCount()
	{
		return this->alive;
	}

	bool Space::registerEmitter(Emitter* emitter)
	{
		if (this->emitters.contains(emitter))
		{
			return false;
		}
		this->emitters += emitter;
		emitter->_setSpace(this);
		return true;
	}

	bool Space::unregisterEmitter(Emitter* emitter)
	{
		if (!this->emitters.contains(emitter))
		{
			return false;
		}
		this->emitters -= emitter;
		emitter->_setSpace(NULL);
		return true;
	}

	bool Space::addAffector(Affector* affector)
	{
		return this->_addAffector(affector);
	}

	bool Space::removeAffector(Affector* affector)
	{
		return this->_removeAffector(affector);
	}

	Emitter* Space::getEmitter(chstr name)
	{
		foreach (Emitter*, it, this->emitters)
		{
			if ((*it)->getName() == name)
			{
				return (*it);
			}
		}
		return NULL;
	}

	hstr Space::getProperty(chstr name, bool* property_exists)
	{
		if (property_exists != NULL)
		{
			*property_exists = true;
		}
		if (name == "pre_update")		return this->getPreUpdate();
		if (name == "fixed_time_step")	return this->getFixedTimeStep();
		if (name == "up")				return gvec3_to_hstr(this->getUp());
		return SpaceObject::getProperty(name, property_exists);
	}

	bool Space::setProperty(chstr name, chstr value)
	{
		if		(name == "pre_update")		this->setPreUpdate(value);
		else if	(name == "fixed_time_step")	this->setFixedTimeStep(value);
		else if	(name == "up")				this->setUp(hstr_to_gvec3(value));
		else return SpaceObject::setProperty(name, value);
		return true;
	}

	void Space::reset()
	{
		foreach_q (Particle*, it, this->particles)
		{
			delete (*it);
		}
		this->particles.clear();
		this->alive = 0;
		this->started = false;
		foreach (Emitter*, it, this->emitters)
		{
			(*it)->reset();
			(*it)->clearParticles();
		}
	}

	void Space::update(float timeDelta)
	{
		if (!this->enabled)
		{
			return;
		}
		if (this->fixedTimeStep <= 0.0f)
		{
			this->_updateInternal(timeDelta);
		}
		else
		{
			float time = timeDelta + this->_lastTimeFraction;
			int steps = (int)(time / this->fixedTimeStep);
			this->_lastTimeFraction = time - steps * this->fixedTimeStep;
			for_iter (i, 0, steps)
			{
				this->_updateInternal(this->fixedTimeStep);
			}
		}
	}

	void Space::_updateInternal(float timeDelta)
	{
		if (!this->started)
		{
			this->started = true;
			if (this->preUpdate > 0.0f)
			{
				int count = (int)(this->preUpdate * 10.0f);
				for_iter (i, 0, count)
				{
					this->update(PRE_UPDATE_TIME_STEP);
				}
				if (this->preUpdate != (float)count)
				{
					float remaining = hmodf(this->preUpdate, PRE_UPDATE_TIME_STEP);
					if (remaining > 0.0f)
					{
						this->update(remaining);
					}
				}
			}
		}
		// update all particles
		this->alive = 0;
		foreach_q (Particle*, it, this->particles)
		{
			(*it)->timer += timeDelta;
			if (!(*it)->isDead())
			{
				this->_movement.set(0.0f, 0.0f, 0.0f);
				foreach (Affector*, it2, this->affectors)
				{
					(*it2)->_setSpace(this);
					(*it2)->update((*it), timeDelta, this->_movement);
				}
				(*it)->position += this->_movement + (*it)->direction * timeDelta;
				++this->alive;
			}
		}
		// updated emitters (create new particles as well)
		foreach (Emitter*, it, this->emitters)
		{
			(*it)->update(timeDelta);
		}
		if (this->particles.size() > 0)
		{
			int i = 0;
			// remove all expired particles
			for_iterx (i, 0, this->particles.size())
			{
				if (!this->particles[i]->isDead())
				{
					break;
				}
				delete this->particles[i];
			}
			if (i > 0)
			{
				this->particles.remove_first(i);
			}
		}
	}

	void Space::finish()
	{
		foreach (Emitter*, it, this->emitters)
		{
			(*it)->setRunning(false);
		}
	}

	void Space::draw(gvec3 point)
	{
		if (this->visible)
		{
			foreach (Emitter*, it, this->emitters)
			{
				(*it)->draw(point, this->up);
			}
		}
	}

	void Space::draw(gvec2 offset, april::Color color)
	{
		if (this->visible && color.a > 0)
		{
			if (color == april::Color::White)
			{
				foreach (Emitter*, it, this->emitters)
				{
					(*it)->draw(offset);
				}
			}
			else
			{
				foreach (Emitter*, it, this->emitters)
				{
					(*it)->draw(offset, color);
				}
			}
		}
	}

	void Space::drawAffectors()
	{
		foreach (Affector*, it, this->affectors)
		{
			(*it)->_setSpace(this);
			(*it)->draw();
		}
	}
	
	void Space::_addNewParticle(float timeDelta)
	{
		this->particles += this->_particle;
		++this->alive;
		this->_movement.set(0.0f, 0.0f, 0.0f);
		foreach (Affector*, it, this->affectors)
		{
			(*it)->_setSpace(this);
			(*it)->update(this->_particle, timeDelta, this->_movement);
		}
		this->_particle->position += this->_movement + this->_particle->direction * timeDelta;
	}

}


