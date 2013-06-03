/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include "AffectorCallbackAffector.h"
#include "Particle.h"

namespace aprilparticle
{
	namespace Affectors
	{
		CallbackAffector::CallbackAffector(chstr name) : Affector(name)
		{
			this->affectionCallback = NULL;
		}
		
		CallbackAffector::CallbackAffector(void (*function)(CallbackAffector*, Particle*, float, gvec3&), chstr name) : Affector(name)
		{
			this->affectionCallback = function;
		}
		
		CallbackAffector::~CallbackAffector()
		{
		}
		
		Affector* CallbackAffector::createInstance(chstr name)
		{
			return new CallbackAffector(name);
		}

		hstr CallbackAffector::getProperty(chstr name, bool* property_exists)
		{
			if (property_exists != NULL)
			{
				*property_exists = true;
			}
			if (this->properties.has_key(name))	return this->properties[name];
			return Affector::getProperty(name, property_exists);
		}

		bool CallbackAffector::setProperty(chstr name, chstr value)
		{
			if (!Affector::setProperty(name, value))
			{
				this->properties[name] = value;
			}
			return true;
		}

		void CallbackAffector::update(Particle* particle, float k, gvec3& movement)
		{
			if (this->affectionCallback != NULL)
			{
				(*this->affectionCallback)(this, particle, k, movement);
			}
		}

	}

}

