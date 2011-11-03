/// @file
/// @author  Domagoj Cerjan
/// @author  Boris Mikic
/// @version 1.2
/// 
/// @section LICENSE
/// 
/// This program is free software; you can redistribute it and/or modify it under
/// the terms of the BSD license: http://www.opensource.org/licenses/bsd-license.php
/// 
/// @section DESCRIPTION
/// 
/// Represents a color affector. This class is more light-weight compared to multi-color affector.

#ifndef APRILPARTICLE_COLOR_AFFECTOR_H
#define APRILPARTICLE_COLOR_AFFECTOR_H

#include <april/Color.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	class aprilparticleExport ColorAffector : public Affector
	{
	public:
		ColorAffector();
		ColorAffector(april::Color endColor);
		ColorAffector(april::Color startColor, april::Color endColor);
		~ColorAffector();

		april::Color getStartColor() { return this->startColor; }
		void setStartColor(april::Color value) { this->startColor = value; }
		april::Color getEndColor() { return this->endColor; }
		void setEndColor(april::Color value) { this->endColor = value; }
				
		void update(Particle* particle, float k);

	protected:
		april::Color startColor;
		april::Color endColor;

	private:
		float _ratio;

	};
	
}

#endif
