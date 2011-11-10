/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
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
		
		CallbackAffector::CallbackAffector(void (*function)(CallbackAffector*, Particle*, float), chstr name) : Affector(name)
		{
			this->affectionCallback = function;
		}
		
		CallbackAffector::~CallbackAffector()
		{
		}
		
		void CallbackAffector::update(Particle* particle, float k)
		{
			if (this->affectionCallback != NULL)
			{
				(*this->affectionCallback)(this, particle, k);
			}
		}

	}

}

