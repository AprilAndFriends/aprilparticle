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
/// Represents a color changer. This class is more light-weight compared to multi-color changer.

#ifndef APRILPARTICLE_COLOR_CHANGER_H
#define APRILPARTICLE_COLOR_CHANGER_H

#include <april/Color.h>
#include <hltypes/hstring.h>

#include "Affector.h"

namespace aprilparticle
{
	class Particle;

	namespace Affectors
	{
		class aprilparticleExport ColorChanger : public Affector
		{
		public:
			ColorChanger();
			ColorChanger(april::Color endColor);
			ColorChanger(april::Color startColor, april::Color endColor);
			~ColorChanger();

			april::Color getStartColor() { return this->startColor; }
			void setStartColor(april::Color value) { this->startColor = value; }
			void setStartColor(chstr value) { this->startColor.set(value); }
			april::Color getEndColor() { return this->endColor; }
			void setEndColor(april::Color value) { this->endColor = value; }
			void setEndColor(chstr value) { this->endColor.set(value); }
				
			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float k);

		protected:
			april::Color startColor;
			april::Color endColor;

		private:
			float _ratio;

		};
	};
}

#endif
