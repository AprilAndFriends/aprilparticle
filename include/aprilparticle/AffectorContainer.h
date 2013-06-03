/// @file
/// @author  Boris Mikic
/// @version 2.0
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents an objects that contains affectors.

#ifndef APRILPARTICLE_AFFECTOR_CONTAINER_H
#define APRILPARTICLE_AFFECTOR_CONTAINER_H

#include <hltypes/harray.h>
#include <hltypes/hltypesUtil.h>
#include <hltypes/hstring.h>

#include "aprilparticleExport.h"

namespace aprilparticle
{
	class Affector;

	class aprilparticleExport AffectorContainer
	{
	public:
		AffectorContainer();
		AffectorContainer(const AffectorContainer& other);
		virtual ~AffectorContainer();

		HL_DEFINE_GET(harray<Affector*>, affectors, Affectors);

		Affector* getAffector(chstr name);
		template <class T>
		T* getAffector(chstr name)
		{
			return dynamic_cast<T*>(this->getAffector(name));
		}

	protected:
		harray<Affector*> affectors;

		bool _addAffector(Affector* affector);
		bool _removeAffector(Affector* affector);

	};
}

#endif
