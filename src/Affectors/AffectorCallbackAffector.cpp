/// @file
/// @version 3.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://opensource.org/licenses/BSD-3-Clause

#include "AffectorCallbackAffector.h"
#include "Particle.h"

namespace aprilparticle
{
	namespace Affectors
	{
		CallbackAffector::CallbackAffector(chstr name) :
			Affector(name)
		{
			this->type = "Callback";
			this->affectionCallback = NULL;
		}
		
		CallbackAffector::CallbackAffector(void (*function)(CallbackAffector*, Particle*, float, gvec3f&), chstr name) :
			Affector(name)
		{
			this->type = "Callback";
			this->affectionCallback = function;
		}

		CallbackAffector::CallbackAffector(const CallbackAffector& other) :
			Affector(other)
		{
			this->properties = other.properties;
			this->affectionCallback = other.affectionCallback;
		}
		
		Affector* CallbackAffector::createInstance(chstr name)
		{
			return new CallbackAffector(name);
		}

		hstr CallbackAffector::getProperty(chstr name)
		{
			if (this->properties.hasKey(name))
			{
				return this->properties[name];
			}
			return Affector::getProperty(name);
		}

		bool CallbackAffector::setProperty(chstr name, chstr value)
		{
			if (!Affector::setProperty(name, value))
			{
				this->properties[name] = value;
			}
			return true;
		}

		void CallbackAffector::update(Particle* particle, float timeDelta, gvec3f& movement)
		{
			if (this->affectionCallback != NULL)
			{
				(*this->affectionCallback)(this, particle, timeDelta, movement);
			}
		}

	}

}

