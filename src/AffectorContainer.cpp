/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php

#include <hltypes/harray.h>
#include <hltypes/hstring.h>

#include "AffectorContainer.h"
#include "Affector.h"

namespace aprilparticle
{
	AffectorContainer::AffectorContainer()
	{
	}
	
	AffectorContainer::AffectorContainer(const AffectorContainer& other)
	{
	}
	
	AffectorContainer::~AffectorContainer()
	{
	}

	bool AffectorContainer::_addAffector(Affector* affector)
	{
		if (this->affectors.contains(affector))
		{
			return false;
		}
		this->affectors += affector;
		return true;
	}

	bool AffectorContainer::_removeAffector(Affector* affector)
	{
		if (!this->affectors.contains(affector))
		{
			return false;
		}
		this->affectors -= affector;
		return true;
	}

	Affector* AffectorContainer::getAffector(chstr name)
	{
		foreach (Affector*, it, this->affectors)
		{
			if ((*it)->getName() == name)
			{
				return (*it);
			}
		}
		return NULL;
	}

}


