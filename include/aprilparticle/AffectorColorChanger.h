/// @file
/// @author  Domagoj Cerjan
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
/// Represents a color changer. This class is more light-weight compared to multi-color changer.

#ifndef APRILPARTICLE_COLOR_CHANGER_H
#define APRILPARTICLE_COLOR_CHANGER_H

#include <april/Color.h>
#include <gtypes/Vector3.h>
#include <hltypes/hltypesUtil.h>
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
			ColorChanger(chstr name = "");
			ColorChanger(april::Color endColor, chstr name = "");
			ColorChanger(april::Color startColor, april::Color endColor, chstr name = "");
			~ColorChanger();
			static Affector* createInstance(chstr name = "");

			HL_DEFINE_GETSET(april::Color, startColor, StartColor);
			void setStartColor(chstr value) { this->startColor.set(value); }
			HL_DEFINE_GETSET(april::Color, endColor, EndColor);
			void setEndColor(chstr value) { this->endColor.set(value); }
				
			hstr getProperty(chstr name, bool* property_exists = NULL);
			bool setProperty(chstr name, chstr value);

			void update(Particle* particle, float k, gvec3& movement);

		protected:
			april::Color startColor;
			april::Color endColor;

		private:
			float _ratio;

		};
	};
}

#endif
